/** @module ForeignCode
 *  @description Contains methods for spawning child processes for other programming languages. This is not an exhaustive implementation.
 * It could provide more functionality here, but because this is not the real purpose, just only the necessary methods are available.
 */
import child_process from 'child_process';
import logger from './Logger';

const defaults = {
  cwd: global.appRoot,
  env: process.env,
  encoding: 'utf8',
  maxBuffer: 2000 * 1024,
};

/**
 * @function Shell
 * @param  {String}  path    Path to the Jar file to execute
 * @description Instantiates a new Shell object.
 * @returns {Object}  A Shell object
 */
export function ShellHelper(path) {
  var obj = new Shell(path);
  return obj;
}

/**
 * @class Java
 * @param {String} path  Path to the Jar file to execute
 * @classdesc Class for executing Jar files
 */
export class Shell {
  constructor(path) {
    this.path = path;
    this.args = [];
    this.output = '';
    this.process = null;
    this.isRunning = false;
  }

  /**
   * @function data
   * @param  {Array} data Array of command line arguments
   * @description Add command line arguments to the execution of the file
   * @memberof module:ForeignCode~Shell
   * @returns {Object} Java object instance
   */
  data(data) {
    data.forEach(element => {
      this.args.push(element.toString());
    });
    return this;
  }

  /**
   * @function call
   * @param  {Function} onEndCallback Function which handles if there is no more data on the buffer to consume
   * @param  {Function} onExitCallback Function to handle if the prcocess exits
   * @description Spawns a child process and executes the specified Jar file asynchronously
   * @memberof module:ForeignCode~Shell
   * @returns {void}
   */
  call(onEndCallback, onExitCallback) {
    logger.info(`Spawning ${this.path} ${this.args.join(' ')}`);
    this.process = child_process.spawn(this.path, this.args, defaults);
    this.isRunning = true;

    this.process.stdout.on('data', data => {
      this.output += '' + data;
      if (data.toString().endsWith('\n')) {
        onEndCallback(this.output.trim());
        this.output = '';
      }
    });

    this.process.stdout.on('end', () => {
      if (typeof onEndCallback === 'function') {
        onEndCallback(this.output);
      }
      this.output = '';
    });

    this.process.stderr.on('data', data => {
      logger.error(`stderr: ${data}`);
    });

    this.process.on('exit', code => {
      this.isRunning = false;
      logger.info(`child process ${this.process.pid} exited with code ${code}`);
      if (typeof onExitCallback === 'function') {
        onExitCallback(this.output);
      }
    });
  }

  writeToProcess(text) {
    this.process.stdin.write(text);
    this.process.stdin.write('\n');
  }

  /**
   * @function callSync
   * @description Spawns a child process and executes the specified Jar file synchronously
   * @returns {String} The stdout of the process
   * @memberof module:ForeignCode~Shell
   */
  callSync() {
    this.process = child_process.spawnSync(this.path, this.args, defaults);
    if (this.process.stderr) logger.error(`stderr: ${process.stderr}`);
    return this.process.stdout;
  }

  /**
   * @function kill
   * @description Kills the executed process by sending 'SIGINT' to it. It is like pressing CTRL+C
   * @memberof module:ForeignCode~Shell
   */
  kill() {
    if (this.isRunning) {
      logger.log('info', 'Manually killing ' + this.process.pid);
      this.process.kill('SIGINT');
    }
  }
}
