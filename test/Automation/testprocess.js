var cp = new ChildProcess();

cp.started.connect(function() {
    log('my process started.');
});
cp.stateChanged.connect(function(newstate) {
    log('process stated changed to ' + newstate);
});
cp.error.connect(function(code) {
    log('error: ', code);
});
cp.finished.connect(function(exitCode, exitStatus) {
    log('exitCode: ', exitCode, 'exitStatus', exitStatus);
});
cp.readyReadStandardOutput.connect(function() {
    log('计算程序的输出：');
    log(cp.readAllStandardOutput());
});

//cp.spawn('notepad', ['e:/aa.txt'], {cwd: 'e:/', env: { my: 'hihi'}});
//cp.spawn('notepad', ['e:/aa.txt'], {detached: true, cwd: 'e:/', env: { my: 'hihi'}});
cp.spawn('e:/code/simcube-auto/test/console.exe');
var go = new Buffer('go\n');
cp.write(go);
cp.write('go');
cp.write('\n');
cp.write(go);
cp.write(go);
cp.write('end\n');
//cp.write(new Buffer('en'));
