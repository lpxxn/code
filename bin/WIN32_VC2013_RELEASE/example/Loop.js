// 循环仿函的示例，包含一个嵌套循环
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
    process.sleep(1);
};

var sf2 = new rom.AutomationScript('sf2', loop);
var dpar4 = new rom.DoubleParameter('data1', rom.Input, sf2);
var dpar5 = new rom.DoubleParameter('data2', rom.Output, sf2);
var dpar6 = new rom.DoubleParameter('data3', rom.Output, sf2);

sf2.scriptFunction = function() {
    self.data2.value = 2*Math.sin(self.data1.value) + Math.random();
    self.data3.value = 3*Math.cos(self.data1.value) + Math.random();
    process.sleep(1);
};

sf2.createLinkByParameters(dpar4, dpar1);

var loop2 = new rom.Loop('loop2', loop);
var sf3 = new rom.AutomationScript('sf3', loop2);
var dpar7 = new rom.DoubleParameter('data1', rom.Input, sf3);
var dpar8 = new rom.DoubleParameter('data2', rom.Output, sf3);

sf3.scriptFunction = function() {
    self.data2.value = 4 * Math.sin(self.data1);
};

loop2.setLoopParameterByGeneralParameter(dpar7);
loop2.setFrom(0);
loop2.setIncrement(0.1);
loop2.setTo(3.14);

loop.setLoopParameterByGeneralParameter(dpar1);
loop.setFrom(0);
loop.setIncrement(0.1);
loop.setTo(3.14);

loop.beforeOneIterationFunction = function() {
    log('before: 当前循环次数', self.counter);
};
loop.afterOneIterationFunction = function() {
    log('after: 当前循环次数', self.counter);
};

loop.appendParameterToMonitor(dpar1, rom.TableParameter.X);
loop.appendParameterToMonitor(dpar2, rom.TableParameter.Y);
