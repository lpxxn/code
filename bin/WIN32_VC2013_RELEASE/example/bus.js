rom.model = new rom.Assembly('model', null);
var model = rom.model;

var r0 = new rom.Bus('bus', model);
var r1 = new rom.Script('sr1', r0);
var p1 = new rom.DoubleParameter('data1', rom.Input, r1);
var p2 = new rom.DoubleParameter('data2', rom.Output, r1);
r1.scriptCode = "data2 = Math.sin(data1) + Math.random();";

var r2 = new rom.Script('sr2', r0);
var p3 = new rom.DoubleParameter('data1', rom.Input, r2);
var p4 = new rom.DoubleParameter('data2', rom.Output, r2);
r2.scriptCode = "data2 = Math.cos(data1) + Math.random();";

var r3 = new rom.Script('sr3', r0);
var p5 = new rom.DoubleParameter('data1', rom.Input, r3);
var p6 = new rom.DoubleParameter('data2', rom.Output, r3);
r3.scriptCode = "data2 = Math.sin(data1) + Math.random();";

var r4 = new rom.Script('sr4', r0);
var p7 = new rom.DoubleParameter('data1', rom.Input, r4);
var p8 = new rom.DoubleParameter('data2', rom.Output, r4);
r4.scriptCode = "data2 = Math.cos(data1) + Math.random();";

r4.createLinkByParameters(p7, p6);
r3.createLinkByParameters(p5, p4);
r2.createLinkByParameters(p3, p2);

r0.firstRunIndex = 0;
//r0.nextRuns = [1, 0];
//r0.priorities = [0, 0];
//r0.runConditions = ["true", "true"];
var resultExpr = p2.fullName + " + " + p4.fullName + " + " + p6.fullName + " + " + p8.fullName;
r0.runConditionsCode = [p2.fullName + "< 0.5;", p2.fullName + ">=1.1 && " + p2.fullName + " <1.2;", p2.fullName + "<=1.", p2.fullName + "<1.5"];
r0.stopConditionCode = "(" + resultExpr + " -4) < 1e-2;";
r0.setRunConditionFunction(0, function() {
    var i = 1;
    i++;
    log("hello");
});
r0.setRunConditionFunction(1, function() {
    var i = 2;
    i++;
    log("hello");
});
r0.setRunConditionFunction(2, function() {
    var i = 4;
    i++;
    log("hello");
});
r0.setRunConditionFunction(3, function() {
    var i = 5;
    i++;
    log("hello");
});