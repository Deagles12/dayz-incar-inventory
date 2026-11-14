modded class VicinityItemManager
{
    protected EntityAI m_ICI_LastVehicle;

    override void RefreshVicinityItems()
    {
        // Rebuild the vanilla list first, including removal after leaving a seat.
        super.RefreshVicinityItems();

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        EntityAI vehicle;
        if (player && player.IsAlive() && !player.IsUnconscious() && !player.IsRestrained() && !player.IsRestrainPrelocked() && !player.IsRestrainStarted())
        {
            HumanCommandVehicle command = player.GetCommand_Vehicle();
            if (command)
                vehicle = EntityAI.Cast(command.GetTransport());
        }

        if (vehicle && vehicle.GetInventory())
        {
            // The occupied vehicle may fail vanilla cone/obstruction/distance discovery.
            // Insert only this vehicle; do not extend access to nearby vehicles.
            array<EntityAI> items = GetVicinityItems();
            if (items && items.Find(vehicle) == -1)
                items.Insert(vehicle);
        }

        if (vehicle != m_ICI_LastVehicle)
        {
            if (vehicle)
                Print(string.Format("[InCarInventory 1.2] occupied-vehicle=%1 visible=%2 cargoVisible=%3 attachments=%4", vehicle.GetType(), vehicle.IsInventoryVisible(), vehicle.CanDisplayCargo(), vehicle.GetInventory().AttachmentCount()));
            else if (m_ICI_LastVehicle)
                Print("[InCarInventory 1.2] occupied-vehicle cleared");
            m_ICI_LastVehicle = vehicle;
        }
    }
}