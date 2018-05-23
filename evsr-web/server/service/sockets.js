/** @module Sockets */
import * as fs from 'fs';

import { ShellHelper } from '../util/Shell';
import SocketIO from 'socket.io';
import logger from '../util/Logger';

var evsr = ShellHelper('./evsr/bin/evsr_run');

var currentConfig = {};
var ready = false;

/**
 * @function listenToSockets
 * @param  {Object} httpServer Express http server instance
 * @description Start up webserver sockets to allow a bidirectional communication between client and server.
 * <br />  <strong>Possible events: </strong>
 *      <ul>
 *        <li>client:requestStatus - client requests the current status of the application</li>
 *        <li>client:init - inits the evsr application with given configuration</li>
 *        <li>client:search - inits a search with the given search_value using the running evsr application</li>
 *        <li>client:stop - client stops the evsr application</li>
 *        <li>client:getEvalData - clients want to display evaluation data</li>
 *      </ul>
 * @return {void}
 */
export function listenToSockets(httpServer) {
  var io = new SocketIO(httpServer);

  /**
   * @function returnStatus
   * @param {*} socket
   * @description Sends the current status to a specific socket or all connected clients
   */
  function returnStatus(socket) {
    if (socket) {
      socket.emit('server:returnStatus', {
        running: evsr.isRunning,
        ready: ready,
        config: currentConfig,
      });
    } else {
      io.emit('server:returnStatus', {
        running: evsr.isRunning,
        ready: ready,
        config: currentConfig,
      });
    }
  }

  /**
   * @function returnResult
   * @param {object} data
   * @description Sends the search result to every client connected
   */
  function returnResult(data) {
    io.emit('server:searchResult', data);
  }

  logger.info('Setting up websocket');

  io.on('connection', function(socket) {
    var socket = socket;

    logger.info(`Client is connected ${socket.id}`);

    /**
     * Event when the client requests the current status of the application
     */
    socket.on('client:requestStatus', data => {
      logger.info('Client requested current status');
      returnStatus(socket);
    });

    /**
     * Event when the client wants to start the evsr application
     */
    socket.on('client:init', data => {
      logger.info('Initialize evsr with: ' + JSON.stringify(data));
      currentConfig = data;

      // Append start paramaters, delete old ones if needed
      if (evsr.args.length != 0) {
        evsr.args = new Array();
      }

      evsr.data([
        '--tiers',
        currentConfig.number_tiers,
        '--dimensions',
        currentConfig.lsh_dim,
        '--collection-path',
        './evsr/data/collection.docs',
        '--stopword-path',
        './evsr/data/stopwords.large',
        '--word-embeddings-path',
        './evsr/data/glove.6B.300d.txt',
        '--server',
        '--seed',
        '1'
      ]);

      //Call the process
      evsr.call(
        data => {
          data = data.toString();
          // Grab ready string
          if (data.substring(0, 7) === '[Ready]') {
            logger.info('evsr ready');
            ready = true;
            returnStatus();
          }

          // Grab result and parse the JSON
          if (data.substring(0, 14) === '[Your result]:') {
            try {
              const results = JSON.parse(data.substring(14, data.length + 2));
              //console.log(results);
              io.emit('server:searchResult', results);
            } catch (err) {
              logger.error(err);
              socket.emit('server:searchError', { error: err.toString() });
            }
          }
        },
        result => {
          // Executed if the process stops
          ready = false;
          returnStatus(socket);
        },
      );
      returnStatus(socket);
    });

    /**
     * Event when the client wants to search something
     */
    socket.on('client:search', data => {
      logger.info('Issue search with value: ' + JSON.stringify(data));

      if (evsr.isRunning) {
        evsr.writeToProcess(JSON.stringify(data));
      } else {
        socket.emit('server:error', {
          message: 'evsr is not running',
        });
      }
    });

    /**
     * Event when the client wants to stop the evsr application
     */
    socket.on('client:stop', data => {
      logger.info('Stop evsr application');
      evsr.kill();
      ready = false;
      returnStatus();
    });

    /**
     * Event when the client wants to load evaluation data
     */
    socket.on('client:getEvalData', data => {
      logger.info('Client requested evaluation data');
      let file = `${__dirname}/../eval/aggregated-per-mode/Test_VSM_RAND_Seed1_2.json-aggregated.json`;
      fs.readFile(file, 'utf8', function(err, data) {
        if (err) throw err;
        socket.emit('server:returnEvalData', {
          filename: file.split('/').pop(),
          data: JSON.parse(data),
        });
      });
    });
  });
}
