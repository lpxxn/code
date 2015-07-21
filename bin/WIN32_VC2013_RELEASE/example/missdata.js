rom.model = new rom.Assembly('model');
var model = rom.model;

var dir = __dirname;

var sf = new rom.AutomationScript('sf', model);
sf.scriptFunction = function() {
    log('runtime');
    log(self.dir);
};
