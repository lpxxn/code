rom.model = new rom.Assembly('model', null);
var model = rom.model;

var loop = new rom.Loop('条件判断子流程', model);
var script0 = new rom.AutomationScript('计算程序', loop);
var input1 = new rom.IntegerParameter('input1', rom.Input, script0);
var flag = new rom.DoubleParameter('flag', rom.Output, script0);
script0.scriptFunction = function() {
	self.flag.value = Math.random() * 100;
};

var sw = new rom.Switch('分支选择', loop);
var srArr = new Array(4);
for (var i = 0; i < 4; i++) {
    srArr[i] = new rom.AutomationScript('sr'+(i+1), sw);
    var data1 = new rom.DoubleParameter('data1', rom.Input, srArr[i]);
    var data2 = new rom.DoubleParameter('data2', rom.Output, srArr[i]);
    srArr[i].scriptFunction = function() {
        self.data2.value = self.data1 + 2;
    };
    srArr[i].preProcessingFunction = function() {
        self.data1.value = 4.2;
    };
    srArr[i].postProcessingFunction = function() {
        var counter = self.counter;
        if (!counter) {
            counter = self.dynamicModifyModel("new rom.IntegerParameter('counter', rom.Output, self)");
            counter.description = 'dynamic counter';
        }
        counter.value += 1;
    };
}
sw.setBranchCondition(srArr[0], flag.fullName + ' <= 25 && 0 <= ' + flag.fullName );
sw.setBranchCondition(srArr[1], flag.fullName + ' <= 50 && 25 < ' + flag.fullName );
sw.setBranchCondition(srArr[2], flag.fullName + ' <= 75 && 50 < ' + flag.fullName );
sw.setBranchCondition(srArr[3], flag.fullName + ' <= 100 && 75 < ' + flag.fullName);

// 循环类型
loop.loopType = rom.Loop.For;
// 循环参数
loop.setLoopParameterByGeneralParameter(input1);
loop.setFrom(0);
loop.setTo(100);
loop.setIncrement(1);

