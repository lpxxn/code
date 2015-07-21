
// 绘制曲线图，保存为文件
// 直接操作数组对象
var t1 = Date.now();
var x = [], y1 = [], y2 = [], y3 = [];
for (var i = 0, j = 0; i < 6.28; i += 0.01, j++) {
    x.push(i);
    y1.push(Math.sin(i));
    y2.push(Math.cos(i));
    y3.push(y1[j] * y2[j]);
}
var t2 = Date.now();
log('时间差: ', t2 - t1, '毫秒');
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.pdf', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.jpg', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.bmp', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.svg', {title: "测试曲线图"});

// 使用lodash
var t1 = Date.now();
var x = _.range(0, 6.28, .01);
var y1 = _.map(x, function(num) { return Math.sin(num);});
var y2 = _.map(x, function(num) { return Math.cos(num);});
var y3 = _.map(x, function(num) { return Math.sin(num) * Math.cos(num);});
var t2 = Date.now();
log('时间差: ', t2 - t1, '毫秒');
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.pdf', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.jpg', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.bmp', {title: "测试曲线图"});
ui.saveGraph([x, y1, y2, y3], 'f:/temp/output/test.svg', {title: "测试曲线图"});

// 使用lodash
var t1 = Date.now();
var y = _.map(_.range(0, 6.28, .01), function(num) { 
    var sin = Math.sin(num);
    var cos = Math.cos(num);
    return [num, sin, cos, sin*cos];
    });
var data = _.unzip(y);
var t2 = Date.now();
log('时间差: ', t2 - t1, '毫秒');
ui.saveGraph(data, 'f:/temp/output/test', {title: "测试曲线图"});
ui.saveGraph(data, 'f:/temp/output/test.pdf', {title: "测试曲线图"});
ui.saveGraph(data, 'f:/temp/output/test.jpg', {title: "测试曲线图"});
ui.saveGraph(data, 'f:/temp/output/test.bmp', {title: "测试曲线图"});
ui.saveGraph(data, 'f:/temp/output/test.svg', {title: "测试曲线图"});




