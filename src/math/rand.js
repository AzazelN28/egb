console.log('#pragma once\n')

console.log('uint8_t rand_table[256] = {');
let col = 0
let row = []
for (const x of Array.from(new Array(256), (_, x) => x).sort((a,b) => Math.random() - 0.5)) {
  row.push(x)
  col++;
  if (col === 16) {
    console.log('  ' + row.join(', ') + ',')
    row.length = 0
    col = 0
  }
}
console.log('};');
