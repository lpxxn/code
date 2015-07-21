var model = new rom.Assembly('model');
rom.model = model;

var cs = new rom.CoSimulation('cs', model);
var fmu = new rom.FMUProxy('fmu', cs);
fmu.fmuFilePath = 'BouncingBall2_cs.fmu';

// 不设定计算终止时间，由变量的计算结果判定是否结束联合仿真
cs.stopTimeDefined = false;
cs.startTime = 0;
cs.stepTime = 0.05;
cs.currentTime = 0;
// 球的高度小于0.1时停止计算
cs.stopConditionFunction = function() {
    model.cs.fmu.HIGHT < 0.01;
};
