rom.model = new rom.Assembly('model', null);
var model = rom.model;

var as = new rom.AutomationScript('as', model);

var data1 = new rom.DoubleParameter('data1', rom.Input, as);
var data2 = new rom.DoubleParameter('data2', rom.Output, as);

as.scriptFunction = function() {
    var abc = 10; 
    self.data2.value = abc + self.data1.value + 10*Math.random();
    var group = self.group;
    if (!group) {
        group = self.dynamicModifyModel("new rom.GroupParameter('group', self)");
        data3 = self.dynamicModifyModel(function() {
            group.description = '组参数的描述';
            new rom.DoubleParameter('data3', rom.Input, group);
        });
        data3.description = '双精度浮点数参数';
    }
    data3.value = Math.random()*10;
    var bb = 23;
    var cc = 23;
};
