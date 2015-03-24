rom.model = new rom.Assembly('model', null);
var model = rom.model;

log(__filename);
log(__dirname);

var as = new rom.AutomationScript('习大大', model);
as.scriptFunction = function() {
    ui.uiInteractive(function() {
        var loader = new ui.UiLoader();
        var dlg = loader.load(path.join(__dirname, 'test.ui'), process.mainWindow);
        dlg.sfNameLineEdit.text = self.name;
        dlg.textBrowser.plainText = rom.$.formatJavaScriptCode('{' + model.toJSON() + '}');
        log(dlg.exec());
    });
};
var script = new rom.AutomationScript('我', model);
new rom.AutomationScript('小强', model);

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
/*
 *自定义模型元素（可以是任意仿函与参数）的说明
 *通过定义模型元素的actionListFunction函数，编写JavaScript脚本的方式定义模型元素的动作列表菜单。
 *该函数是一个无参函数，函数内部只定义一个数组对象，数组的每一个元素描述一个动作项，也即一个菜单项。
 *数组的元素是一个对象，该对象可以有下列键值对：
 *menuItem，字符串，动作名称，界面上是菜单的字符串，当名称是seperator字符串时，则表示菜单上的一个分隔条，此时忽略其它键值。
 *statusTip，字符串，当鼠标移动到该菜单时，在窗口状态栏中显示的动作描述信息，这个在右键弹出菜单下无效。
 *icon，字符串，指定菜单的图标文件，可以是绝对路径，也可以使用$$代表应用程序所在目录的images的子目录。
 *actor，函数对象，带一个参数self，参数名称不一定非是self，也可以用其它有效变量名，但用self是一个好习惯，
 *它表示当前模型元素对象。函数内部可访问软件中的JavaScript各种接口。
 *defaultActor，布尔值，为true表示该动作在菜单上是缺省的，点击工具栏上的按钮即可执行该动作，无需打开菜单。
 */
// 仿函对象的自定义动作列表菜单函数定义
script.actionListFunction = function() {
    [{
        menuItem: '菜单一',
        statusTip: '我是谁',
        icon: '$$/icemcfd.png',
        actor: function(self) {
            log('我是谁？');
            //仿函的previous属性代表当前仿函所在层级的上一个仿函对象，next则是下一个仿函对象
            log('我是', self.previous.next.previous.next.name);
        }
    }, {
        menuItem: 'seperator'
    }, {
        menuItem: '菜单二',
        defaultActor: true,
        statusTip: '你兄弟是谁',
        icon: '$$/ds.png',
        actor: function(self) {
            log('你兄弟是谁？');
            log('我哥是', self.previous.name, '我弟是', self.next.name);
        }
    }, {
        menuItem: '菜单三(&F)',
        statusTip: '动态界面',
        actor: function(self) {
            var loader = new ui.UiLoader();
            var dlg = loader.load(path.join(__dirname, 'test.ui'), process.mainWindow);
            dlg.sfNameLineEdit.text = self.name;
            dlg.textBrowser.plainText = rom.$.formatJavaScriptCode('{' + model.toJSON() + '}');
            log(dlg.exec());
        }
    }];
};

// 参数对象的自定义动作列表菜单函数定义
data3.actionListFunction = function() {
    [{
        menuItem: '参数菜单',
        actor: function(self) {
            log(self.fullName);
        }
    }, {
        menuItem: '缺少函数定义'
    }]
};