// 测试总线驱动仿函的功能
rom.model = new rom.Assembly('model', null);
var model = rom.model;

var r0 = new rom.Bus('bus', model);
var data1 = new rom.TableParameter('data1', rom.Output, r0);
data1.appendColumn(rom.TableParameter.Double);
data1.appendColumn(rom.TableParameter.Double);
data1.setColumnNameAt(0, 'A');
data1.setColumnUnitAt(0, '秒');
data1.setColumnNameAt(1, 'B');
data1.setColumnUnitAt(1, '帕');
data1.setColumnSignificanceAt(1, rom.TableParameter.Y);

var r1 = new rom.AutomationScript('sr1', r0);
var p1 = new rom.DoubleParameter('time', rom.Input, r1);
var p2 = new rom.DoubleParameter('pressure1', rom.Output, r1);
r1.scriptFunction = function() {
    process.sleep(1);
    self.time.value = self.time + 0.1;
    log("time is:" + self.time);
    self.pressure1.value = Math.pow(self.time, 0.2) - 0.6 * self.time;
    log("pressure1 is:" + self.pressure1);
    self.parentSimfun.parameters.data1.appendRowAt(0, self.time.value);
    self.parentSimfun.parameters.data1.appendRowAt(1, self.pressure1.value);
}

var r2 = new rom.AutomationScript('sr2', r0);
var p3 = new rom.DoubleParameter('time', rom.Input, r2);
var p4 = new rom.DoubleParameter('pressure2', rom.Output, r2);
r2.scriptFunction = function() {
    self.previous.parameters.time.value = self.previous.parameters.time + 0.1;
    log("time is:" + self.time);
    self.pressure2.value = Math.pow(self.time - 1, 0.4) - 0.4 * self.time;
    log("pressure2 is:" + self.pressure2);
    self.parentSimfun.parameters.data1.appendRowAt(0, self.time.value);
    self.parentSimfun.parameters.data1.appendRowAt(1, self.pressure2.value);
}

var r3 = new rom.AutomationScript('sr3', r0);
var p5 = new rom.DoubleParameter('time', rom.Input, r3);
var p6 = new rom.DoubleParameter('pressure3', rom.Output, r3);
r3.scriptFunction = function() {
    self.previous.previous.parameters.time.value = self.previous.previous.parameters.time + 0.1;
    log("time is:" + self.time);
    self.pressure3.value = Math.pow(self.time - 2, 0.6) - 0.33 * self.time;
    log("pressure3 is:" + self.pressure3);
    self.parentSimfun.parameters.data1.appendRowAt(0, self.time);
    self.parentSimfun.parameters.data1.appendRowAt(1, self.pressure3);
}

var r4 = new rom.AutomationScript('sr4', r0);
var p7 = new rom.DoubleParameter('time', rom.Input, r4);
var p8 = new rom.DoubleParameter('pressure4', rom.Output, r4);
r4.scriptFunction = function() {
    self.previous.previous.previous.parameters.time.value = self.previous.previous.previous.parameters.time + 0.1;
    log("time is:" + self.time);
    self.pressure4.value = Math.pow(self.time - 5, 0.8) - 0.2 * self.time;
    log("pressure4 is:" + self.pressure4);
    self.parentSimfun.parameters.data1.appendRowAt(0, self.time);
    self.parentSimfun.parameters.data1.appendRowAt(1, self.pressure4);
}

r4.createLinkByParameters(p7, p1);
r3.createLinkByParameters(p5, p1);
r2.createLinkByParameters(p3, p1);

r0.firstRunIndex = 0;
r0.priorities = [0, 1, 2, 3];
r0.stopConditionCode = p1.fullName + " >= 7.99 && " + p1.fullName + " <= 8.01;";
r0.setRunConditionFunction(0, function() {
    self.sr1.time.value >= 0 && self.sr1.pressure1.value >= 0;
});
r0.setRunConditionFunction(1, function() {
    self.sr2.previous.parameters.time.value >= 1 && self.sr2.pressure2.value >= 0 && self.sr2.pressure2.value <= self.sr1.pressure1.value;
});
r0.setRunConditionFunction(2, function() {
    self.sr3.previous.previous.parameters.time.value >= 1.3 && self.sr3.pressure3.value >= 0;
});
r0.setRunConditionFunction(3, function() {
    self.sr4.time.value >= 5 && self.sr4.pressure4.value >= 0 && self.sr4.pressure4.value <= self.sr3.pressure3.value;
});
