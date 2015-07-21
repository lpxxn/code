/*! \file
*   \brief 支持FMI标准的仿函注册与创建代码.
*   \author 谭立方
*   \date 2015
*   \version 1.0
*   \copyright 2011-2015 PERA Global
*/

log(qsTranslate("AutoScript", "Import ") + __extension__ + qsTranslate("AutoScript", " plugin..."));
__setupPackage__("rom.autofmi");

function createFMUProxy(model, parentSf) {
    var sf = new rom.FMUProxy(model.properties.id, parentSf);
    rom.$.setSimfunProperties(sf, model.properties);
    sf.fmuFilePath = model.properties.fmuFilePath;

    return sf;
}

function createCoSimulation(model, parentSf) {
    var sf = new rom.CoSimulation(model.properties.id, parentSf);
    rom.$.setSimfunProperties(sf, model.properties);
    sf.stopConditionCode = model.properties.stopCondition;
    return sf;
}

function registerSimfuns() {
    if ('CoSimulation' in rom.library.simfuns)
        throw new Error(qsTranslate('AutoScript', 'CoSimulation has been in existence.'));

    rom.library.simfuns['CoSimulation'] = {
        name : qsTranslate('AutoScript', 'CoSimulation'),
        extension : __extension__,
        factory : createCoSimulation,
        composite : true,
        graphicsItem : 'LoopItem',
        tabpages : [
            { menuItem : qsTranslate('AutoScript', 'History Data Browsing'), icon : 'history.png', actor : 'IterDataBrowsing', constraint : 1},
            { menuItem : qsTranslate('AutoScript', 'Realtime Data Browsing'), icon : 'realtime.png', actor : 'RealtimeDataBrowsing', constraint : 2}
        ],
        icon : 'cosimulation.svg',
        group : qsTranslate('AutoScript', 'Functional Mock-up Units'),
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'The CoSimulation SimFunction is used to execute FMUS.'),
        os : ['windows', 'linux']
    };

    if ('FMUProxy' in rom.library.simfuns)
        throw new Error(qsTranslate('AutoScript', 'FMUProxy has been in existence.'));

    rom.library.simfuns['FMUProxy'] = {
        name : qsTranslate('AutoScript', 'FMUProxy'),
        extension : __extension__,
        factory : createFMUProxy,
        composite : false,
        //tools : [{menuItem : qsTranslate('AutoScript', 'Model Data Extract Editor...'), actor : 'ScriptEditor'}],
        graphicsItem : 'Item',
        manualCreate : false,
        icon : 'fmuproxy.svg',
        group : qsTranslate('AutoScript', 'Functional Mock-up Units'),
        parents : ['CoSimulation'],
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'The FMUProxy SimFunction is used to run FMU.'),
        os : ['windows', 'linux']
    };
}

function createIntegerVariable(name, parameter, sfOrGroup, parObj) {
    if (!parObj)
        parObj = new rom.IntegerFMUVariable(name, parameter['type'], sfOrGroup);
    if ('value' in parameter) {
        parObj.value = parameter['value'];
    } else {
        parObj.value = 0;
    }
    rom.$.setModelElementProperties(parObj, parameter);

    if ('properties' in parameter) {
        rom.$.setNumericProperties(parameter, parObj);
    }
    rom.$.setEnumProperties(parameter, parObj);
    rom.$.setNumberCustomFormat(parameter, parObj);

    return parObj;
}

function createRealVariable(name, parameter, sfOrGroup, parObj) {
    if (!parObj)
        parObj = new rom.RealFMUVariable(name, parameter['type'], sfOrGroup);
    if ('value' in parameter) {
        parObj.value = parameter['value'];
    } else {
        parObj.value = 0;
    }
    rom.$.setModelElementProperties(parObj, parameter);

    if ('properties' in parameter) {
        rom.$.setNumericProperties(parameter, parObj);
    }
    rom.$.setEnumProperties(parameter, parObj);
    rom.$.setNumberCustomFormat(parameter, parObj);

    return parObj;
}

function createBooleanVariable(name, parameter, sfOrGroup, parObj) {
    if (!parObj)
        parObj = new rom.BooleanFMUVariable(name, parameter['type'], sfOrGroup);
    parObj.value = parameter['value'];
    rom.$.setModelElementProperties(parObj, parameter);
    return parObj;
}

function createStringVariable(name, parameter, sfOrGroup, parObj) {
    if (!parObj)
        parObj = new rom.StringFMUVariable(name, parameter['type'], sfOrGroup);
    parObj.value = parameter['value'];
    rom.$.setModelElementProperties(parObj, parameter);
    rom.$.setEnumProperties(parameter, parObj);
    return parObj;
}

function registerParameters() {
    if ('IntegerFMUVariable' in rom.library.parameters)
        throw new Error(qsTranslate('AutoScript', 'IntegerFMUVariable has been in existence.'));
    rom.library.parameters['IntegerFMUVariable'] = {
        name : qsTranslate('AutoScript', 'Integer Variable'),
        factory : createIntegerVariable,
        group : qsTranslate('AutoScript', 'Base Single Value Parameter'),
        numeric : true,
        icon : 'integer.svg',
        extension : __extension__,
        plugin : 'AutoFMIGUI',
        manualCreate : false,
        //tools : [{menuItem : '编辑器...', actor : 'IntegerEditor'}],
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'Integer Variable of FMU.')
    };

    if ('RealFMUVariable' in rom.library.parameters)
        throw new Error(qsTranslate('AutoScript', 'RealFMUVariable has been in existence.'));
    rom.library.parameters['RealFMUVariable'] = {
        name : qsTranslate('AutoScript', 'Real Variable'),
        factory : createRealVariable,
        group : qsTranslate('AutoScript', 'Base Single Value Parameter'),
        numeric : true,
        icon : 'double.svg',
        extension : __extension__,
        plugin : 'AutoFMIGUI',
        manualCreate : false,
        //tools : [{menuItem : '编辑器...', actor : 'IntegerEditor'}],
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'Real Variable of FMU.')
    };

    if ('BooleanFMUVariable' in rom.library.parameters)
        throw new Error(qsTranslate('AutoScript', 'BooleanFMUVariable has been in existence.'));
    rom.library.parameters['BooleanFMUVariable'] = {
        name : qsTranslate('AutoScript', 'Boolean Variable'),
        factory : createBooleanVariable,
        group : qsTranslate('AutoScript', 'Base Single Value Parameter'),
        numeric : true,
        icon : 'bool.svg',
        extension : __extension__,
        plugin : 'AutoFMIGUI',
        manualCreate : false,
        //tools : [{menuItem : '编辑器...', actor : 'IntegerEditor'}],
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'Boolean Variable of FMU.')
    };

    if ('StringFMUVariable' in rom.library.parameters)
        throw new Error(qsTranslate('AutoScript', 'StringFMUVariable has been in existence.'));
    rom.library.parameters['StringFMUVariable'] = {
        name : qsTranslate('AutoScript', 'String Variable'),
        factory : createStringVariable,
        group : qsTranslate('AutoScript', 'Base Single Value Parameter'),
        numeric : true,
        icon : 'string.svg',
        extension : __extension__,
        plugin : 'AutoFMIGUI',
        manualCreate : false,
        //tools : [{menuItem : '编辑器...', actor : 'IntegerEditor'}],
        author : qsTranslate('AutoScript', 'peraglobal'),
        description : qsTranslate('AutoScript', 'String Variable of FMU.')
    };
}

__postInit__ = function() {
    log(qsTranslate('AutoScript', 'FMI is Imported'));
    log(qsTranslate('AutoScript', 'Register FMI SimFunction'));
    registerSimfuns();
    registerParameters();
}
