rom.model = new rom.Assembly('model', null);
var model = rom.model;

var loop = new rom.Loop('loop', model);
loop.loopType = rom.Loop.For;

var sf1 = new rom.AutomationScript('sf1', loop);
var dpar1 = new rom.DoubleParameter('data1', rom.Input, sf1);
var dpar2 = new rom.DoubleParameter('data2', rom.Output, sf1);
var dpar3 = new rom.DoubleParameter('data3', rom.Output, sf1);

sf1.scriptFunction = function() {
    self.data2.value = 2*Math.sin(self.data1.value) + 0.5*Math.random();
    self.data3.value = 4*Math.cos(self.data1.value) + 0.5*Math.random();
};

var sf2 = new rom.AutomationScript('sf2', loop);
var dpar4 = new rom.DoubleParameter('data1', rom.Input, sf2);
var dpar5 = new rom.DoubleParameter('data2', rom.Output, sf2);
var dpar6 = new rom.DoubleParameter('data3', rom.Output, sf2);

sf2.scriptFunction = function() {
    self.data2.value = 2*Math.sin(self.data1.value) + Math.random();
    self.data3.value = 3*Math.cos(self.data1.value) + Math.random();
};

sf2.createLinkByParameters(dpar4, dpar1);

loop.setLoopParameterByGeneralParameter(dpar1);
loop.setFrom(0);
loop.setIncrement(0.1);
loop.setTo(3.14);
