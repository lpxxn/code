/*
使用Loop与AutomationScript模拟方波与锯齿波的生成，可用于测试浏览迭代过程数据。
*/
rom.model = new rom.Assembly('model', null);
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
loop.setIncrement(0.01);
loop.setTo(6.28);