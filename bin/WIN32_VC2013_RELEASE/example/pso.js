// create model
rom.model = new rom.Assembly('model');
var model = rom.model;

var pso = new rom.PSO('pso', model);
var sf = new rom.AutomationScript('sf', pso);
var x1 = new rom.DoubleParameter('x1', rom.Input, sf);
var x2 = new rom.DoubleParameter('x2', rom.Input, sf);
var z = new rom.DoubleParameter('z', rom.Output, sf);
sf.scriptFunction = function() {
    var X1 = self.x1;
    var X2 = self.x2;
    self.z.value = 60 / (1 + (X1 + 1) * (X1 + 1) + (X2 - 3) * (X2 - 3)) + 20 / (1 + (X1 - 1) * (X1 - 1) + (X2 - 3) * (X2 - 3)) + 30 / (1 + X1 * X1 + (X2 + 4) * (X2 + 4))
};

pso.appendDesignVariable(x1.fullName, rom.Optimization.LowerUpper, -5, 5);
pso.appendDesignVariable(x2.fullName, rom.Optimization.LowerUpper, -5, 5);
pso.appendObjective(z.fullName, rom.Optimization.Maximize);
