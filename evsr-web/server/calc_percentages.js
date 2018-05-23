let fs = require('fs');


let aggregated = fs.readFileSync('./eval/aggregated-per-mode/Test_VSM_RAND_Seed1.json-aggregated.json');

let aggregatedJSON = JSON.parse(aggregated);



console.log("                     Cluster                     Cluser_Rand                     Cluster_W2V                     TieredIndex                     TieredIndex_RAND                     TieredIndex_W2V                     VanillaVSM                     VanillaVSM_RAND                     VanillaVSM_W2V") 



aggregatedJSON.map((mode) => {
  console.log(`${mode.name}`);
  aggregatedJSON.map((inner_mode) => {
    let string = `${inner_mode.name} `;
    string += `${((inner_mode.average_rnt - mode.average_rnt)/inner_mode.average_rnt) * 100 }   `


    console.log(string)
  })
})  