let fs = require('fs');


let aggregated = fs.readFileSync('./eval/aggregated-per-mode/Test_VSM_RAND_Seed1.json-aggregated.json');

let aggregatedJSON = JSON.parse(aggregated);


aggregatedJSON.map((mode) => {
  console.log(mode.name);
  console.log(mode.average_rnt);

  aggregatedJSON.map((inner_mode) => {
    console.log(`${inner_mode.name} ${(mode.average_rnt/inner_mode.average_rnt)*100}`)
  })
})  