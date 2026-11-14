modded class MissionGameplay
{
    override void OnInit()
    {
        super.OnInit();
        Print("[InCarInventory 1.2] client mission loaded");
    }

    override void ShowInventory()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            player.ICI_LogState("inventory-open-request");
            HumanCommandVehicle command = player.GetCommand_Vehicle();
            if (command)
            {
                EntityAI vehicle = EntityAI.Cast(command.GetTransport());
                if (vehicle && vehicle.GetInventory())
                    Print(string.Format("[InCarInventory 1.2] inventory-vehicle type=%1 visible=%2 effectiveCargoVisible=%3 attachments=%4", vehicle.GetType(), vehicle.IsInventoryVisible(), ICI_VehicleVisibility.Cargo(vehicle), vehicle.GetInventory().AttachmentCount()));
            }
        }
        super.ShowInventory();
        Print(string.Format("[InCarInventory 1.2] inventory-menu-open=%1", GetUIManager().FindMenu(MENU_INVENTORY) != null));
    }
}