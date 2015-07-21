rom.model = new rom.Assembly('model', null);
var model = rom.model;

var loopSf = new rom.Loop('loop', model);
loopSf.loopType = rom.Loop.While;

var r2 = new rom.AutomationScript('R2', loopSf);
var iv1 = new rom.IntegerParameter('iv1', rom.Input, r2);
var iv2 = new rom.IntegerParameter('iv2', rom.Input, r2);
var iv3 = new rom.IntegerParameter('iv3', rom.Output, r2);
var iv4 = new rom.IntegerParameter('iv4', rom.Output, r2);
r2.scriptFunction = function() {
    self.iv3.value = self.iv1 * self.iv1 + 2*self.iv1 + 1;
    self.iv4 = self.iv1 + self.iv2;
};

var inputs = new rom.ReferenceArrayParameter('inputs', false, loopSf);
inputs.appendReferenceParameter(iv1, true);
inputs.appendReferenceParameter(iv2, true);

var initials = new rom.ReferenceArrayParameter('initials', true, loopSf);
initials.appendReferenceParameter(1, false);
initials.appendReferenceParameter(2, false);

var feedbacks = new rom.ReferenceArrayParameter('feedbacks', true, loopSf);
feedbacks.appendReferenceParameter(iv3, true);
feedbacks.appendReferenceParameter(iv4, true);

loopSf.setCondition('model.loop.R2.iv3<100 || model.loop.R2.iv4<100');
//loopSf.setCondition('model.R1.R2.iv3<100 && model.R1.R2.iv4<100');
loopSf.inputs = inputs;
loopSf.initials = initials;
loopSf.feedbacks = feedbacks;

