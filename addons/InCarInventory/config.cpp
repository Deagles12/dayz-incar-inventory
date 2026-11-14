class CfgPatches
{
    class InCarInventory
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class CfgMods
{
    class InCarInventory
    {
        dir = "InCarInventory";
        picture = "InCarInventory\\mod_icon.paa";
        action = "";
        hideName = 0;
        hidePicture = 0;
        name = "InCarInventory";
        credits = "You";
        author = "You";
        version = "1.2";
        type = "mod";
        dependencies[] = {"World", "Mission"};

        class defs
        {
            class missionScriptModule { files[] = {"InCarInventory/scripts/5_Mission"}; };
            class worldScriptModule
            {
                files[] = {"InCarInventory/scripts/4_World"};
            };
        };
    };
};
