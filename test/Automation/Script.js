rom.model = new rom.Assembly('model', null);
var model = rom.model;

log(__FILE__);

var script = new rom.AutomationScript('script', model);

var data1 = new rom.DoubleParameter('data1', rom.Input, script);
var data2 = new rom.DoubleParameter('data2', rom.Output, script);
var data3 = new rom.DoubleArrayParameter('data3', rom.Input, [2, 3], script);
data3.array = [1, 2, 3, 4, 5, 6];

script.scriptFunction = function() {
    self.data2.value = self.data1 + Math.random() * 100;
    self.data3.setValue([1, 1], 11.11);
    log(self.data1);
    log(self.data2);
    log(self.data3);
};

script.preProcessingFunction = function() {
    self.data1.value = 12;
};
