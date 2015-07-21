// 注意此文件中的绝对路径，运行时可能需要修改
log(__filename);
log(__dirname);

// 建模
rom.model = new rom.Assembly('model');
var model = rom.model;
var loop = new rom.Loop('傅里叶级数', model);
loop.loopType = rom.Loop.For;

var sf1 = new rom.AutomationScript('方波', loop);
var dpar1 = new rom.DoubleParameter('data1', rom.Input, sf1);
var dpar2 = new rom.DoubleParameter('data2', rom.Output, sf1);
var dpar3 = new rom.DoubleParameter('data3', rom.Output, sf1);
var dpar4 = new rom.DoubleParameter('data4', rom.Output, sf1);
var dpar5 = new rom.DoubleParameter('data5', rom.Output, sf1);

sf1.scriptFunction = function() {
    self.data2.value = 4 * Math.sin(self.data1.value) / Math.PI;
    self.data3.value = self.data2 + 4 * Math.sin(3 * self.data1) / (3 * Math.PI);
    self.data4.value = self.data3 + 4 * Math.sin(5 * self.data1) / (5 * Math.PI);
    self.data5.value = self.data4 + 4 * Math.sin(7 * self.data1) / (7 * Math.PI);
};

var sf2 = new rom.AutomationScript('锯齿波', loop);
var dpar6 = new rom.DoubleParameter('data1', rom.Input, sf2);
var dpar7 = new rom.DoubleParameter('data2', rom.Output, sf2);
var dpar8 = new rom.DoubleParameter('data3', rom.Output, sf2);
var dpar8 = new rom.DoubleParameter('data4', rom.Output, sf2);
var dpar10 = new rom.DoubleParameter('data5', rom.Output, sf2);

sf2.scriptFunction = function() {
    self.data2.value = 2 * Math.sin(self.data1) / (-Math.PI);
    self.data3.value = self.data2 + 2 * Math.sin(2 * self.data1) / (2 * Math.PI);
    self.data4.value = self.data3 + 2 * Math.sin(3 * self.data1) / (-3 * Math.PI);
    self.data5.value = self.data4 + 2 * Math.sin(4 * self.data1) / (4 * Math.PI);
};

sf2.createLinkByParameters(dpar6, dpar1);
loop.setLoopParameterByGeneralParameter(dpar1);
loop.setFrom(0);
loop.setIncrement(0.1);
loop.setTo(6.28);

// 保存模型为rom文件
var romfile = 'd:/temp/output/abc.rom';
fs.unlink(romfile);
rom.saveModel(romfile);
// 设置当前模型标识, 必须
rom.currentModelId = 1;

log('模型开始同步运行');
rom.runModelSync();
log('模型运行完毕');
// 设置当前模型的结果标识
rom.currentResultId = 1;

(function() {
    var resultId = rom.currentResultId;
    var allPar = [dpar1, dpar2, dpar3, dpar4, dpar5];
    for (var i = 0; i < allPar.length; i++)
        rom.retrieveParameter(allPar[i], resultId);
    while (1) {
        var count = 0;
        for (i = 0; i < allPar.length; i++) {
            var ok = rom.waitForRetrieveFinished(allPar[i], resultId, 100);
            count += ok;
        }
        if (count == allPar.length)
            break;
    }

    var allParValues = [];
    for (i = 0; i < allPar.length; i++) {
        allParValues.push(rom.resultList(allPar[i], resultId));
    }

    // 数据输出为文本文件
    var inputfilename = 'd:/temp/output/output.txt';
    var inputfile = new File(inputfilename);
    if (!inputfile.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    for (var idx = 0, size = allParValues[0].length; idx < size; ++idx) {
        for (i = 0; i < allPar.length; i++) {
            inputfile.writeString(allParValues[i][idx] + (i != allPar.length - 1 ? '\t' : ''));
        }
        inputfile.writeString('\n');
    }
    inputfile.close();

    // 数据输出为Excel文件
    var xlsx = new office.Xlsx('d:/temp/output/output.xlsx');
    xlsx.addSheet('方波数据');
    for (idx = 1, size = allParValues[0].length; idx <= size; ++idx) {
        xlsx.write(idx, 1, allParValues[0][idx - 1]);
        xlsx.write(idx, 2, allParValues[1][idx - 1]);
        xlsx.write(idx, 3, allParValues[2][idx - 1]);
        xlsx.write(idx, 4, allParValues[3][idx - 1]);
        xlsx.write(idx, 5, allParValues[4][idx - 1]);
    }
    xlsx.save();

    // 数据输出为曲线图
    ui.saveGraph([allParValues[0], allParValues[4]] , 'd:/temp/output/output.png', {title: '方波'});
    
    // 数据输出为Word文件
    var docx = new office.Docx();
    docx.addHeading('傅里叶级数数据报表');
    docx.addHeading('数据列表', 2);
    var table = docx.addTable(allParValues[0].length, 5);
    for (idx = 0, size = allParValues[0].length; idx < size; ++idx) {
        var cells = table.rowCells(idx);
        var colcount = cells.count();
        for (var col = 0; col < colcount; col++){
            var cell = cells.at(col);
            cell.addText(allParValues[col][idx])
        }
    }
    docx.addPageBreak();
    docx.addHeading('曲线图', 2);
    docx.addPicture('d:/temp/output/output.png');
    docx.save('d:/temp/output/output.docx');
})();

