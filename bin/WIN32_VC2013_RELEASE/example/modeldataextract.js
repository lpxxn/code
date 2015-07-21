rom.model = new rom.Assembly('model', null);
var model = rom.model;

var mde = new rom.ModelDataExtract('模型数据提取', model);

mde.extractFunction = function() {
    var t = 'model.傅里叶级数.方波.data1';
    var d2 = 'model.傅里叶级数.方波.data2';
    var d3 = 'model.傅里叶级数.方波.data3';
    var d4 = 'model.傅里叶级数.方波.data4';
    var d5 = 'model.傅里叶级数.方波.data5';
    var resultId = rom.currentResultId()
    var allPar = [t, d2, d3, d4, d5];
    for (var i = 0; i < allPar.length; i++)
        rom.retrieveData(allPar[i], resultId);
    while (1) {
        var count = 0;
        for (i = 0; i < allPar.length; i++) {
            var ok = rom.waitForRetrieveDataFinished(allPar[i], resultId, 100);
            count += ok;
        }
        if (count == allPar.length)
            break;
    }

    var inputfilename = path.join(outputDir, 'output.txt');
    var inputfile = new File(inputfilename);
    if (!inputfile.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    var allParValues = [];
    for (i = 0; i < allPar.length; i++) {
        allParValues.push(rom.resultData(allPar[i], resultId));
    }
    for (var idx = 0, size = allParValues[0].length; idx < size; ++idx) {
        for (i = 0; i < allPar.length; i++) {
            inputfile.writeString(allParValues[i][idx] + (i != allPar.length - 1 ? '\t' : ''));
        }
        inputfile.writeString('\n');
    }

    inputfile.close();
		
    // 把提取到的文件信息写入一个文本文件中
		var resultfilename = path.join(outputDir, 'result.json');
		var resultfile = new File(resultfilename);
    if (!resultfile.open(File.WriteOnly | File.Text))
        throw 'open file error.';
    var result = {
        'extractedfiles': [{'file': inputfilename, 'desc': '提取的数据文件'}]
    };
    resultfile.writeString(JSON.stringify(result));
};
