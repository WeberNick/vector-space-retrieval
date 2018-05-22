var fs = require('fs');
var path = require('path');

let eval_dir = `${__dirname}/eval`;

fs.readdir(eval_dir, (err, files) => {
  if (err) {
    console.error('Could not list the directory.', err);
    process.exit(1);
  }
  files.forEach((file, index) => {
    var fromPath = path.join(eval_dir, file);
    if (fs.lstatSync(fromPath).isDirectory()) {
      console.log(`${file} is a directoy !`);
    } else {
      let fileData = require(fromPath);

      var aggregated = new Array();

      fileData.map(queryType => {
        let aggregatedTypeData = new Object();
        aggregatedTypeData.name = queryType.name;

        aggregatedTypeData.modes = new Array();

        queryType.modes.map(mode => {
          let aggregatedMode = {};

          aggregatedMode.name = mode.name;
          aggregatedMode.map = mode.map;

          aggregatedMode.average_rnt =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_rnt;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_acc =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_acc;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_avp =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_avp;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_dcg =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_dcg;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_fms =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_fms;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_pre =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_pre;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / mode.queries.length
              : 0;

          aggregatedMode.average_rec =
            mode.queries.reduce((sum, current) => {
              return sum + current.perf_rec;
            }, 0) /
              mode.queries.length >
            0
              ? mode.queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / mode.queries.length
              : 0;

          aggregatedTypeData.modes.push(aggregatedMode);
          aggregatedTypeData.modes.sort((a, b) => {
            if (a.name > b.name) return 1; // if (a.name.split('_').pop() > b.name.split('_').pop()) return 1;
          });
        });
        aggregated.push(aggregatedTypeData);
      });

      if (!fs.existsSync(`${__dirname}/eval/aggregated/`)) {
        fs.mkdirSync(`${__dirname}/eval/aggregated/`);
      }

      fs.writeFileSync(
        `${__dirname}/eval/aggregated/${file.split('/').pop()}-aggregated.json`,
        JSON.stringify(aggregated, null, 2),
      );
    }
  });
});
