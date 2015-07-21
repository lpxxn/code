rom.model = new rom.Assembly('model', null);
var model = rom.model;

var script = new rom.AutomationScript('script', model);

var data1 = new rom.IntegerParameter('data1', rom.Input, script);
var data2 = new rom.DoubleParameter('data2', rom.Output, script);

script.scriptFunction = function() {
    self.data2.value = self.data1 + Math.random()*100;
};

data1.respondChangedFunction = function() {
    log(self.fullName);
    log(self.value);
    if (self.value === 1) {
        var sf = self.owningSimfun;
        var ip = new rom.IntegerParameter('int1', rom.Input, sf);
        ip.value = 12;
    }
}

