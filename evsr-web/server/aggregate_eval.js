var fs = require('fs');
var path = require('path');
var util = require('util')


let eval_dir = `${__dirname}/eval`;




function aggregateQueryTypes(){
  fs.readdir(eval_dir, (err, files) => {
    if (err) {
      console.error('Could not list the directory.', err);
      process.exit(1);
    }
    files.forEach((file, index) => {
      var fromPath = path.join(eval_dir, file);
      if (fs.lstatSync(fromPath).isDirectory() || file.startsWith('.')) {
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
}


function aggregateModes(){
  fs.readdir(eval_dir, (err, files) => {
    if (err) {
      console.error('Could not list the directory.', err);
      process.exit(1);
    }
    files.forEach((file, index) => {
      var fromPath = path.join(eval_dir, file);
      if (fs.lstatSync(fromPath).isDirectory() || file.startsWith('.')) {
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

        
        
        //console.log(util.inspect(aggregated, {showHidden: false, depth: null}))

        let modes = new Array();

        let cluster = new Object();
        cluster.name = "Cluster";
        
        let cluster_rand = new Object();
        cluster_rand.name = "Cluster_RAND";
        
        let cluster_w2v = new Object();
        cluster_w2v.name = "Cluster_W2V";
        

        let tiered_index = new Object();
        tiered_index.name = "TieredIndex";
        
        let tiered_index_rand = new Object();
        tiered_index_rand.name = "TieredIndex_RAND";
        
        let tiered_index_w2v = new Object();
        tiered_index_w2v.name = "TieredIndex_W2V";

        let vanilla_vsm = new Object();
        vanilla_vsm.name = "VanillaVSM";
        
        let vanilla_vsm_rand = new Object();
        vanilla_vsm_rand.name = "VanillaVSM_RAND";
        
        let vanilla_vsm_w2v = new Object();
        vanilla_vsm_w2v.name = "VanillaVSM_W2V";

        
        modes.push(cluster, cluster_rand, cluster_w2v, tiered_index, tiered_index_rand, tiered_index_w2v, vanilla_vsm, vanilla_vsm_rand, vanilla_vsm_w2v);

        aggregated.map((queryType) => {
          queryType.modes.map((mode)=>{
            let foundIndex = modes.findIndex((mode_in_array) => {
              return mode_in_array.name === mode.name;
            });

          
            if (foundIndex >= 0) {
              modes[foundIndex].average_rnt ? modes[foundIndex].average_rnt = (modes[foundIndex].average_rnt + mode.average_rnt) : modes[foundIndex].average_rnt =  mode.average_rnt;
              modes[foundIndex].average_acc ? modes[foundIndex].average_acc = (modes[foundIndex].average_acc + mode.average_acc) : modes[foundIndex].average_acc =  mode.average_acc;
              modes[foundIndex].average_avp ? modes[foundIndex].average_avp = (modes[foundIndex].average_avp + mode.average_avp) : modes[foundIndex].average_avp =  mode.average_avp;
              modes[foundIndex].average_dcg ? modes[foundIndex].average_dcg = (modes[foundIndex].average_dcg + mode.average_dcg) : modes[foundIndex].average_dcg =  mode.average_dcg;
              modes[foundIndex].average_fms ? modes[foundIndex].average_fms = (modes[foundIndex].average_fms + mode.average_fms) : modes[foundIndex].average_fms =  mode.average_fms;
              modes[foundIndex].average_pre ? modes[foundIndex].average_pre = (modes[foundIndex].average_pre + mode.average_pre) : modes[foundIndex].average_pre =  mode.average_pre;
              modes[foundIndex].average_rec ? modes[foundIndex].average_rec = (modes[foundIndex].average_rec + mode.average_rec) : modes[foundIndex].average_rec =  mode.average_rec;
            }
          })
        })

        modes.map((mode) => {
          mode.average_rnt = mode.average_rnt / aggregated.length;
          mode.average_acc = mode.average_acc / aggregated.length;
          mode.average_avp = mode.average_avp / aggregated.length;
          mode.average_dcg = mode.average_dcg / aggregated.length;
          mode.average_fms = mode.average_fms / aggregated.length;
          mode.average_pre = mode.average_pre / aggregated.length;
          mode.average_rec = mode.average_rec / aggregated.length;
        } )



  
        if (!fs.existsSync(`${__dirname}/eval/aggregated-per-mode/`)) {
          fs.mkdirSync(`${__dirname}/eval/aggregated-per-mode/`);
        }
  
        fs.writeFileSync(
          `${__dirname}/eval/aggregated-per-mode/${file.split('/').pop()}-aggregated.json`,
          JSON.stringify(modes, null, 2),
        );
      }
    });
  });  
}

aggregateQueryTypes();
//console.log("hallo")
aggregateModes();