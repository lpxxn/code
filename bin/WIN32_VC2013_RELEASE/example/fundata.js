function fun1(x, y) {
    return 60 / (1 + (x + 1) * (x + 1) + (y - 3) * (y - 3)) + 20 / (1 + (x - 1) * (x - 1) + (y - 3) * (y - 3)) + 30 / (1 + x * x + (y + 4) * (y + 4));
}

function fun2(x, y) {
    return 20 * Math.exp(-0.2 * Math.sqrt((x * x + y * y) / 2)) + Math.exp((Math.cos(2 * Math.PI * x) + Math.cos(2 * Math.PI * y)) / 2) - Math.exp(1) - 20;
}

function writeTextFile(path, fun, x1, x2, y1, y2) {
    var file = new File(path);
    file.open(File.WriteOnly | File.Text);
    var stream = new TextStream(file);
    for (var x = x1; x <= x2; x += .1) {
        for (var y = y1; y <= y2; y += .1) {
            stream.writeString(x + '\t' + y + '\t' + fun(x, y) + '\n');
        }
    }
    file.close();
}

function writeXlsxFile(path, fun, x1, x2, y1, y2) {
    var xlsx = new office.Xlsx(path);
    var row = 1;
    for (var x = x1; x <= x2; x += .1) {
        for (var y = y1; y <= y2; y += .1) {
            xlsx.write(row, 1, x);
            xlsx.write(row, 2, y);
            xlsx.write(row, 3, fun(x, y));
            row++;
        }
    }
    xlsx.save();
}

var t1 = Date.now();
writeTextFile('e:/fun1data.txt', fun1, -5, 5, -5, 5);
var t2 = Date.now();
log('text file fun1 time: ', (t2 - t1)/1000, 'seconds');

t1 = Date.now();
writeTextFile('e:/fun2data.txt', fun2, -10, 10, -10, 10);
t2 = Date.now();
log('text file fun2 time: ', (t2 - t1)/1000, 'seconds');

t1 = Date.now();
writeXlsxFile('e:/fun1data.xlsx', fun1, -5, 5, -5, 5);
t2 = Date.now();
log('xlsx file fun1 time: ', (t2 - t1)/1000, 'seconds');

t1 = Date.now();
writeXlsxFile('e:/fun2data.xlsx', fun2, -10, 10, -10, 10);
t2 = Date.now();
log('xlsx file fun2 time: ', (t2 - t1)/1000, 'seconds');
