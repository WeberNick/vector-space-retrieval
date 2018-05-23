let fs = require('fs');


let aggregated = fs.readFileSync('./eval/aggregated-per-mode/Test_VSM_RAND_Seed1.json-aggregated.json');

let aggregatedJSON = JSON.parse(aggregated);


aggregatedJSON.map((mode) => {
  console.log(mode.name);
})