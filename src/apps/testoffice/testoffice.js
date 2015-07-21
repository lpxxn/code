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

    print('a lign  ', office.Xlsx.Format.AlignRight);
    format.setHorizontalAlignment(office.Xlsx.Format.AlignRight);
    xlsx.write("B3", "abc", format);

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
    var curreentws = xlsx.currentWorksheet();
    print("test1 rowCount", curreentws.rowCount(), "test1 columnCount", curreentws.columnCount());


    xlsx.addSheet("aaa");
    xlsx.copySheet("Sheet1", "页面2");
    
    curreentws = xlsx.currentWorksheet();
    curreentws.writeString("A8", "aa");
    curreentws.mergeCells('B4:F6');
    print("test1 rowCount", curreentws.rowCount(), "test1 columnCount", curreentws.columnCount());
    curreentws.setColumnWidth(30, 1, 1);
    curreentws.setColumnWidth(30, 3, 5);
    curreentws.setColumnFormat(format, 1, 1);
    curreentws.setColumnHidden(true, 3, 3);
    print("is column hidden", curreentws.isColumnHidden(3));
    //curreentws.setRowFormat(2, 3, format);
    //curreentws.setRowHeight(30, 1, 2);
    //curreentws.setRowHeight(40, 5, 5);
    //curreentws.groupColumns(10, 13);
    //curreentws.groupRows(7, 12);
    //curreentws.setFormulasVisible(true);
    //curreentws.setGridLinesVisible(false);
    //curreentws.setRowColumnHeadersVisible(false);
    //curreentws.setRightToLeft(true);
    curreentws.setWhiteSpaceVisible(true);
    print("isSelected  ", curreentws.isSelected());
    print("is gridLine display", curreentws.isGridLinesVisible());
    curreentws.setRowHidden(2, 2, true);
    print("is row hidden", curreentws.isRowHidden(2));
    
    xlsx.mergeCells('J3:J9', format);
    xlsx.mergeCells('B8:C21');
    xlsx.unmergeCells('B8:C21');
    xlsx.insertImage(5, 5, "e:/light.jpg");
    var img = new office.Xlsx.Image("e:/light.jpg");
    img = img.setSize(80, 80);
    xlsx.insertImage(19, 5, img);

    xlsx.save();
}

function testXlsx2() {
    var xlsx = new office.Xlsx('e:/test2.xlsx');
    xlsx.addSheet('第一页');
    var format = new office.Xlsx.Format();
    format.fontSize = 22;
    for (var row = 1; row < 10; ++row) {
        xlsx.write(row, 1, row);
        xlsx.write(row, 2, row * Math.random());
        xlsx.write(row, 3, '随机数：' + Math.random());
        xlsx.write(row, 4, new Date());
        xlsx.write(row, 5, false);
        xlsx.write(row, 6, true);
    }
    xlsx.setColumnWidth(30, 2);
    xlsx.setColumnWidth(30, 3, 5);
    xlsx.setColumnFormat(format, 2);
    xlsx.setColumnHidden(true, 7, 8);
    print(" column 3. width  ", xlsx.columnWidth(3));
    print(" is column 7 hidden  " , xlsx.isColumnHidden(7));

    xlsx.setRowHeight(42, 3);
    xlsx.setRowHeight(42, 5, 6);
    xlsx.setRowHidden(10, true);
    xlsx.setRowHidden(12, 13, true);
    xlsx.setRowFormat(3, 13, format);

    xlsx.groupRows(4, 6, true);
    xlsx.groupColumns(7, 9, true);

    print("test2 rowCount", xlsx.rowCount(), "test1 columnCount", xlsx.columnCount());
    xlsx.addSheet('第二页', office.Xlsx.ChartSheet);
    xlsx.save();
}

function testXlsx3() {
    var xlsx = new office.Xlsx('e:/test3.xlsx');
    xlsx.addSheet('测试页');
    var ws = xlsx.worksheet('测试页');
    ws.setZerosVisible(false);
    print(" isOutlineSymbolsVisible  ", ws.isOutlineSymbolsVisible());
    //ws.setOutlineSymbolsVisible(false);
    //ws.write(1, 1, 'Hello word');
    //ws.write('B1', 'Hello word');
    //print(ws.read(1, 1));
    //print(ws.read('B1'));
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
    print("test3 rowCount", xlsx.rowCount(), "test1 columnCount", xlsx.columnCount());

    xlsx.save();

    var xlsx2 = new office.Xlsx('e:/test4.xlsx');
    xlsx2.addSheet('测试页');
    var ws2 = xlsx2.worksheet('测试页');
    print("is zerovisible ", ws2.isZerosVisible());
    ws2.setZerosVisible(false);
    print("is zerovisible ", ws2.isZerosVisible());
    ws2.setRulerVisible(true);
    xlsx2.save();
}

function testDocx1() {
    var docx = new office.Docx();
    docx.addHeading('标题');
    docx.addParagraph('测试1');
    docx.addHeading('标题', 2);


    var p = docx.addParagraph("内容");
    //docx.addPicture('e:/light.jpg');
    docx.addPicture('e:/light.jpg', 3);
    p.insertParagraphBefore("Before");
    p.insertParagraphBefore("Before2", office.Docx.ListBullet);
    p.insertParagraphBefore("Number1", office.Docx.ListNumber);
    p.insertParagraphBefore("Number2", office.Docx.ListNumber);
    docx.addParagraph("内容", office.Docx.ListNumber);
    //docx.addPageBreak();
    var run = p.addRun('AddRun');
    run.addText("Text");
    run.setUnderLine(office.Docx.Run.SINGLE)
    p.addText("段落添加");
    run = p.addRun('AddRun段落');
    run.setUnderLine(office.Docx.Run.WAVY_DOUBLE)

    run = p.addRun('内容Run');
    run.setDoubleStrike(true);

    print(run.text);
    run.text = '全新内容';
    print(run.text);

    var alignp = docx.addParagraph("内容");

    var section = docx.addSection();    
    var header = section.defaultHeader();
    var p = header.addParagraph('页眉');    
    header.showNumber(office.Docx.Section.NormalNum);
    var footer = section.defaultFooter();
    footer.addParagraph("BBBBB");
    footer.showNumber(office.Docx.Section.RomanNum);

    var table = docx.addTable(3, 3);
    var cells = table.rowCells(0);
    var cell0 = cells.at(0);
    cell0.addText("内容");

    var cell1 = cells.at(1);
    var cellp = cell1.addParagraph("Paragraph");
    cellp.addRun("MyRun");

    var cells2 = table.rowCells(1);
    var cell10 = cells2.at(2);
    cell10.addTable(3, 3);

    var cell11 = cells2.at(1);
    var p11 = cell11.addParagraph();
    var run11 = p11.addRun();
    run11.addPicture('e:/light.jpg', 4);

    var section2 = docx.addSection();
    var header2 = section2.defaultHeader();
    header2.addParagraph("CCCC");
    header2.showNumber(office.Docx.Section.NormalNum);
    section2.restartNum();
    var footer2 = section2.defaultFooter();
    footer2.addParagraph("DDDD");

    // 所有段落
    var ps = docx.paragraphs();
    print(" paragraphs count : ", ps.count());
    var pat2 = ps.at(5);
    pat2.addRun("PS AddRun");

    var tables = docx.tables();
    print(" tables count : ", tables.count());

    // 所有表格
    var table2 = tables.at(1);
    var frcells2 = table2.rowCells(2);
    var fcell2 = frcells2.at(2);
    fcell2.addParagraph("添加内容");

    docx.save('e:/abc.docx');
}

print('begin test');
//testXlsx1();
//testXlsx2();
//testXlsx3();

testDocx1();

print("end...");
