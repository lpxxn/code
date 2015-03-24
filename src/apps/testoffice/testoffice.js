function testXlsx1() {
    var xlsx = new office.Xlsx('e:/test1.xlsx');
    var format = new office.Xlsx.Format();
    format.fontSize = 22;
    xlsx.write(1, 1, "Hello Qt!", format);
    var cell = xlsx.cellAt(1, 1);
    print("celltype:", cell.cellType);
    print("A1 value: ", cell.value);
    xlsx.write("A2", 12345, format);
    format.fontSize = 33;
    xlsx.write("A3", "=44+33");
    cell = xlsx.cellAt("A3");
    print("A3 hasFormula: ", cell.hasFormula);

    xlsx.write("A4", true, format);
    cell = xlsx.cellAt("A4");
    print("A4 format font size: ", cell.format.fontSize);
    xlsx.write("A5", "http://qt-project.org");
    xlsx.write("A6", new Date());
    cell = xlsx.cellAt("A6");
    print("A6 isDateTime: ", cell.isDateTime, " datetime: ", cell.value);
    xlsx.write("A7", new Date(2014, 12, 22, 6, 30));
    cell = xlsx.cellAt("A7");
    print("A7 isDateTime: ", cell.isDateTime, " datetime: ", cell.value);
    xlsx.copySheet("Sheet1", "页面2");
    xlsx.save();
}

function testXlsx2() {
    var xlsx = new office.Xlsx('e:/test2.xlsx');
    xlsx.addSheet('第一页');
    for (var row = 1; row < 4000; ++row) {
        xlsx.write(row, 1, row);
        xlsx.write(row, 2, row * Math.random());
        xlsx.write(row, 3, '随机数：' + Math.random());
        xlsx.write(row, 4, new Date());
        xlsx.write(row, 5, false);
        xlsx.write(row, 6, true);
    }
    xlsx.addSheet('第二页', office.Xlsx.ChartSheet);
    xlsx.save();
}

function testXlsx3() {
    var xlsx = new office.Xlsx('e:/test3.xlsx');
    xlsx.addSheet('测试页');
    var ws = xlsx.worksheet('测试页');
    ws.write(1, 1, 'Hello word');
    ws.write('B1', 'Hello word');
    print(ws.read(1, 1));
    print(ws.read('B1'));
    ws.writeBlank(2, 1);
    ws.writeBlank('B2');
    print(ws.read(2, 1));
    print(ws.read('B2'));
    ws.writeBool(3, 1, true);
    ws.writeBool('B3', false);
    print(ws.read(3, 1));
    print(ws.read('B3'));
    ws.writeDateTime(4, 1, new Date());
    ws.writeDateTime('B4', new Date());
    print(ws.read(4, 1));
    print(ws.read('B4'));
    ws.writeInlineString(5, 1, 'test');
    ws.writeInlineString('B5', 'test');
    print(ws.read(5, 1));
    print(ws.read('B5'));
    ws.writeNumeric(6, 1, 1.23456789);
    ws.writeNumeric('B6', 2.345467891);
    print(ws.read(6, 1));
    print(ws.read('B6'));
    ws.writeString(7, 1, 'test');
    ws.writeString('B7', 'test');
    print(ws.read(7, 1));
    print(ws.read('B7'));
    xlsx.save();
}

print('begin test');
testXlsx1();
testXlsx2();
testXlsx3();

var docx = new office.Docx();
docx.save('e:/abc.docx');
