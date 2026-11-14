modded class PlayerBase
{
    void ICI_LogState(string eventName)
    {
        if (!GetHumanInventory()) return;
        Print(string.Format("[InCarInventory 1.2] %1 server=%2 vehicle=%3 canOpen=%4 softLocked=%5", eventName, GetGame().IsServer(), IsInVehicle(), GetHumanInventory().CanOpenInventory(), IsInventorySoftLocked()));
    }

    override void OnCommandVehicleStart()
    {
        super.OnCommandVehicleStart();
        if (IsAlive() && !IsUnconscious() && !IsRestrained() && !IsRestrainPrelocked() && !IsRestrainStarted() && GetInventory())
            GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
        ICI_LogState("vehicle-enter");
    }

    override void OnCommandVehicleFinish()
    {
        // Let vanilla release its vehicle lock and restore the hand animation.
        super.OnCommandVehicleFinish();
        ICI_LogState("vehicle-exit");
    }
}