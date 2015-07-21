// 测试与Excel文件的操作
var model = new rom.Assembly('model', null);
rom.model = model;

var script = new rom.AutomationScript('sf1', model);
var p1 = new rom.XlsxFileParameter('data1', rom.Input, script);
p1.fileName = path.join(__dirname, 'aerodata.xlsx');

script.scriptFunction = function() {
    var xlsx = new office.Xlsx(self.data1.fileName);
    var sheetnames = xlsx.sheetNames();
    for (var i = 0, count = sheetnames.length; i < count ; i++) {
        log('页面: ', sheetnames[i]);
        var ws = xlsx.worksheet(sheetnames[i]);
        var r = 12, c = 2;
        var cell = ws.cellAt(r, c);
        if (cell) {
            log('(', r, ', ', c, ') = ', cell.value, ' type: ', cell.cellType);
        } else {
            log('(', r, ', ', c, ') = undefined');
        }

//        while (true) {
//            var v = ws.read(r, c);
//            if (v == undefined)
//            log('(', r, ', ', c, ') = ', ws.read(r, c));
//        }
    }
	xlsx.save();
};

