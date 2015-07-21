// 测试粒子群优化算法

rom.model = new rom.Assembly('model');
var model = rom.model;

var pso = new rom.PSO('pso', model);
var sf = new rom.AutomationScript('sf', pso);
var x = new rom.DoubleParameter('x', rom.Input, sf);
var y = new rom.DoubleParameter('y', rom.Input, sf);
var z = new rom.DoubleParameter('z', rom.Output, sf);
sf.scriptFunction = function() {
    var x = self.x;
    var y = self.y;
    self.z.value = 20 * Math.exp(-0.2 * Math.sqrt((x * x + y * y) / 2)) + Math.exp((Math.cos(2 * Math.PI * x) + Math.cos(2 * Math.PI * y)) / 2) - Math.exp(1) - 20;
};

sf.postProcessingFunction = function() {
//    process.msleep(100);
};

pso.beforeOneIterationFunction = function() {
    log('before: 循环次数', self.counter);
};
pso.afterOneIterationFunction = function() {
    log('after: 循环次数', self.counter);
};



pso.appendDesignVariable(x.fullName, rom.Optimization.LowerUpper, -10, 10);
pso.appendDesignVariable(y.fullName, rom.Optimization.LowerUpper, -10, 10);
pso.appendConstraint(z.fullName, rom.Optimization.Upper, 0, 0, -8);
pso.appendObjective(z.fullName, rom.Optimization.Maximize);
