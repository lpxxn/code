var model = new rom.Assembly('model', null);
rom.model = model;

var script = new rom.AutomationScript('sf1', model);
var p1 = new rom.DoubleParameter('data1', rom.Input, script);
p1.value = 12;
p1.units = '公里';
p1.description = '距离';
var p2 = new rom.DoubleParameter('data2', rom.Output, script);
var p3 = new rom.DoubleParameter('data3', rom.Output, script);

script.scriptFunction = function() {
    self.data2.value = self.data1 + Math.random() * 100;
    self.data3.value = self.data1 * Math.cos(self.data2);
};

var script2 = new rom.AutomationScript('sf2', model);
var p4 = new rom.DoubleParameter('data1', rom.Input, script2);
p4.units = '米';
var p5 = new rom.DoubleParameter('data2', rom.Input, script2);
var p6 = new rom.DoubleParameter('data3', rom.Output, script2);

script2.scriptFunction = function() {
    self.data3.value = (self.data1 + Math.sin(self.data2)) * 100;
};

script2.createLinkByParameters(p5, p2);
script2.createLinkByNames(p4.fullName, p1.fullName + ' * 1000');

