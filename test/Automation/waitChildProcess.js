rom.model = new rom.Assembly('model', null);
var model = rom.model;

var as = new rom.AutomationScript('waitChildProcess', model);
as.scriptFunction = function() {
    log("**********运行run script*************");
    var appPath = "D:\\Qt\\Qt53vc32\\5.3\\msvc2013_opengl\\bin\\launchNotepad.exe";
    var cp = new ChildProcess();    
    cp.childProcessFinished.connect(function() {
        log("***********child process finished.**************");
    });
    cp.spawn(appPath, [], {
        waitForChildProcessFinished: -1
    });
};

as.preProcessingFunction = function() {
    log("**********preProcessing*************");
};

as.postProcessingFunction = function() {
    log("**********postProcessing*************");
};

