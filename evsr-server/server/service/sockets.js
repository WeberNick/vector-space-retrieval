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
 *        <li>client:init - inits the evsr application with given configuration</li>
 *        <li>client:search - inits a search with the given search_value using the running evsr application</li>
 *      </ul>
 * @return {void}
 */
export function listenToSockets(httpServer) {
  var io = new SocketIO(httpServer);

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

  function returnResult(socket, data) {
    console.log(socket.id);
    io.emit('server:searchResult', data);
  }

  logger.info('Setting up websocket');

  io.on('connection', function(socket) {
    var socket = socket;

    logger.info(`Client is connected ${socket.id}`);

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

      // Append start paramaters
      evsr.data([
        '--dimensions',
        '5',
        '--collection-path',
        './evsr/data/collection.docs',
        '--stopword-path',
        './evsr/data/stopwords.large',
      ]);

      //Call the process
      evsr.call(
        data => {
          data = data.toString();
          console.log('-' + data);
          if (data.substring(0, 7) === '[Ready]') {
            logger.info('evsr ready');
            ready = true;
            returnStatus();
          }

          //[Your result]:
          if (data.substring(0, 14) === '[Your result]:') {
            try {
              returnResult(
                socket,
                JSON.parse(data.substring(14, data.length + 2)),
              );
            } catch (err) {
              socket.emit('server:searchError', { error: err.toString() });
            }
          }
        },
        result => {
          console.log('result:' + result);
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

    socket.on('client:stop', data => {
      logger.info('Stop evsr application');
      evsr.kill();
      ready = false;
      returnStatus();
    });

    socket.on('client:getEvalData', data => {
      logger.info('Client is requesting evaluation data');
      fs.readFile(`${__dirname}/../data/graph.json`, 'utf8', function(
        err,
        data,
      ) {
        if (err) throw err;
        socket.emit('server:returnEvalData', JSON.parse(data));
      });
    });
  });
}
