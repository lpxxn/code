/*! \file
*   \brief 微软COM组件的仿函注册与创建代码.
*   \author 谭立方
*   \date 2011-2014
*   \version 1.0
*   \copyright 2011-2014PERA Global
*/

log(qsTranslate("AutoScript", "Import ") + __extension__ + qsTranslate("AutoScript", " plugin..."));
__setupPackage__("rom.autocomobject");
 
function createCOM(model, parentSf) {
    var sf = new rom.COM(model.properties.id, parentSf);
    rom.$.setSimfunProperties(sf, model.properties);
    sf.scriptCode = model.properties.scriptCode;
    return sf;
}

function registerSimfuns() {
    if (os.type().indexOf('Windows') !== -1) {
        if ('COM' in rom.library.simfuns)
            throw new Error(qsTranslate('AutoScript', 'Com has been in existence.'));

        rom.library.simfuns['COM'] = {
            name : qsTranslate('AutoScript', 'COM'),
            extension : __extension__,
            factory : createCOM,
            composite : false,
            //tools : [{menuItem : qsTranslate('AutoScript', 'Model Data Extract Editor...'), actor : 'ScriptEditor'}],
            graphicsItem : 'Item',
            layout : rom.VerticalLayout,
            icon : 'com.svg',
            group : qsTranslate('AutoScript', 'Base SimFunction'),
            author : qsTranslate('AutoScript', 'peraglobal'),
            description : qsTranslate('AutoScript', 'The COM SimFunction is used to interact with Microsoft COM Objects.'),
            os : ['windows']
        };
    }
}
    
__postInit__ = function() {
    log(qsTranslate('AutoScript', 'COM SimFunction is Imported'));
    log(qsTranslate('AutoScript', 'Register COM SimFunction'));
    registerSimfuns();
}
