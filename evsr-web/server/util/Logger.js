/** @module Logger
 *  @description Uses winston as a logger but adding colors to the console.log() output and piping the log messages down to a log file
 *  @see File /server/server.log
 */
import logger from 'winston';
import * as fs from 'fs';

logger.setLevels({
  debug: 0,
  info: 1,
  silly: 2,
  warn: 3,
  error: 4,
});
logger.addColors({
  debug: 'green',
  info: 'cyan',
  silly: 'magenta',
  warn: 'yellow',
  error: 'red',
});

logger.remove(logger.transports.Console);
logger.add(logger.transports.Console, {
  level: 'error',
  colorize: true,
});


if (!fs.existsSync(`${__dirname}/../logs/`)) {
  fs.mkdirSync(`${__dirname}/../logs/`);
}

logger.add(logger.transports.File, {
  filename: `${__dirname}/../logs/server.log`,
});

export default logger;
