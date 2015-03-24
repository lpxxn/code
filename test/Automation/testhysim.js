var model = new rom.Assembly('model');
rom.model = model;

var fluent = new rom.Fluent('流体计算', model);
var dummy = new rom.Dummy('测试驱动', model);
