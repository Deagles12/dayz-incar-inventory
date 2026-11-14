// Visibility only: server transfer rules and script inventory locks remain in force.
class ICI_VehicleVisibility
{
    static bool IsOccupied(EntityAI entity)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!entity || !player || !player.IsAlive() || player.IsUnconscious() || player.IsRestrained() || player.IsRestrainPrelocked() || player.IsRestrainStarted()) return false;
        HumanCommandVehicle command = player.GetCommand_Vehicle();
        return command && command.GetTransport() == entity;
    }
    static bool Cargo(EntityAI entity)
    {
        if (IsOccupied(entity)) return entity.GetInventory() && entity.GetInventory().GetCargo() != null;
        return entity && entity.CanDisplayCargo();
    }
    static bool Category(EntityAI entity, string category)
    {
        return IsOccupied(entity) || (entity && entity.CanDisplayAttachmentCategory(category));
    }
}

modded class AttachmentCategoriesContainer
{
	override void UpdateInterval()
	{
		if (m_Entity)
		{
			int childrenCount = m_Body.Count();
			int displayableChildrenCount = 0;
			
			if (childrenCount > 0)
			{
				int i;
				for (i = m_SlotsCount; i < childrenCount; ++i)
				{
					AttachmentCategoriesRow row = AttachmentCategoriesRow.Cast(m_Body[i]);
					if (row)
					{
						if (ICI_VehicleVisibility.Category(m_Entity, row.GetCategoryIdentifier()))
						{
							if (row.IsDisplayable())
							{
								ShowInSlots(row.GetCategoryIdentifier(), true);
								if (!row.IsVisible())
									row.OnShow();
							}
							else
							{
								ShowInSlots(row.GetCategoryIdentifier(), false);
								if (row.IsVisible())
									row.OnHide();
							}
						}
						else
						{
							ShowInSlots(row.GetCategoryIdentifier(), false);
							if (row.IsVisible())
								row.OnHide();
						}
						row.UpdateInterval();
					}
					else
					{
						ContainerWithCargo cargo = ContainerWithCargo.Cast(m_Body[i]);
						if (cargo)
						{
							if (ICI_VehicleVisibility.Cargo(m_Entity))
							{
								m_CargoSlotsIcon.m_MainWidget.Show(true);
								if (!cargo.IsVisible())
									cargo.OnShow();
							}
							else
							{
								m_CargoSlotsIcon.m_MainWidget.Show(false);
								if (cargo.IsVisible())
									cargo.OnHide();
							}
							cargo.UpdateInterval();
						}
						RecomputeOpenedContainers();
					}
					
				}
				
				//finally update slot category icons
				for (i = 0; i < m_SlotsCount; ++i)
				{
					AttachmentCategoriesSlotsContainer slotsContainer = AttachmentCategoriesSlotsContainer.Cast(m_Body[i]);
					if (slotsContainer.IsDisplayable())
					{
						if (!m_Hidden)
							slotsContainer.OnShow();
						else
							slotsContainer.OnHide();
						
						displayableChildrenCount++; //slot category icons should always count as 'there', regardless of visibility due to closed state of the parent container
					}
					else
					{
						slotsContainer.OnHide();
					}
				}
			}
			
			bool show = displayableChildrenCount > 0 && !m_Entity.GetInventory().IsInventoryLockedForLockType(HIDE_INV_FROM_SCRIPT);
			if (!m_CollapsibleHeader.IsVisible() && show)
			{
				m_CollapsibleHeader.OnShow();
			}
			else if (m_CollapsibleHeader.IsVisible() && !show)
			{
				m_CollapsibleHeader.OnHide();
				HideContent();
			}
			
			GetMainWidget().Show(show);
		}
	}

	override bool CanDisplayAnyCategory()
	{
		AttachmentCategoriesRow row;
		foreach (LayoutHolder holder : m_Body)
		{
			if (Class.CastTo(row, holder) && ICI_VehicleVisibility.Category(m_Entity, row.GetCategoryIdentifier()))
				return true;
		}
		return super.CanDisplayAnyCategory();
	}

	override void UpdateRadialIcon()
	{
		if (m_SlotIcon)
		{
			if (ICI_VehicleVisibility.Cargo(m_Entity) || CanDisplayAnyCategory())
			{
				m_SlotIcon.GetRadialIconPanel().Show(true);
				SetOpenForSlotIcon(!m_Hidden);
			}
			else
			{
				m_SlotIcon.GetRadialIconPanel().Show(false);
			}
		}
	}
}

modded class ContainerWithCargo
{
    override bool IsDisplayable()
    {
        if (ICI_VehicleVisibility.IsOccupied(m_Entity))
            return ICI_VehicleVisibility.Cargo(m_Entity);
        return super.IsDisplayable();
    }
    protected void ICI_RefreshCargoVisibility()
    {
        if (!ICI_VehicleVisibility.IsOccupied(m_Entity) || !m_CargoGrid) return;
        if (m_Entity.GetInventory().IsInventoryLockedForLockType(HIDE_INV_FROM_SCRIPT)) return;
        LockCargo(!ICI_VehicleVisibility.Cargo(m_Entity));
        RecomputeOpenedContainers();
        if (m_SlotIcon)
            m_SlotIcon.GetRadialIconPanel().Show(!m_LockCargo && ItemManager.GetInstance().GetDraggedItem() != m_Entity);
    }
    override void UpdateInterval()
    {
        super.UpdateInterval();
        ICI_RefreshCargoVisibility();
    }
    override void SetEntity(EntityAI entity, int cargo_index = 0, bool immedUpdate = true)
    {
        super.SetEntity(entity, cargo_index, immedUpdate);
        ICI_RefreshCargoVisibility();
    }
}

modded class ContainerWithCargoAndAttachments
{
    protected void ICI_RefreshCargoVisibility()
    {
        if (!ICI_VehicleVisibility.IsOccupied(m_Entity) || !m_CargoGrid) return;
        if (m_Entity.GetInventory().IsInventoryLockedForLockType(HIDE_INV_FROM_SCRIPT) || m_ForcedHide) return;
        if (ICI_VehicleVisibility.Cargo(m_Entity) && !m_CargoGrid.IsVisible())
        {
            ShowCargo();
            RecomputeOpenedContainers();
        }
    }
    override void UpdateInterval()
    {
        super.UpdateInterval();
        ICI_RefreshCargoVisibility();
    }
    override void SetEntity(EntityAI entity, bool immedUpdate = true)
    {
        super.SetEntity(entity, immedUpdate);
        ICI_RefreshCargoVisibility();
    }
}

modded class CargoContainer
{
	override void UpdateHeaderText()
	{
		Widget header;
		string name = m_Entity.GetDisplayName();
		name.ToUpper();
		
		if ( ICI_VehicleVisibility.Cargo(m_Entity) && m_Entity.GetInventory().GetCargoFromIndex(m_CargoIndex) )
		{
			name = name + " (" + GetCargoCapacity().ToString() + "/" + GetMaxCargoCapacity() + ")";
			if ( m_IsAttachment && m_CargoHeader )
			{
				m_FalseHeaderTextWidget.SetText(name);
				float x, y;
				m_FalseHeaderTextWidget.Update();
				m_FalseHeaderTextWidget.GetScreenSize( x, y );
				m_CargoHeader.FindAnyWidget( "grid_container_header" ).SetSize( 1, y + InventoryMenu.GetHeightMultiplied( 10 ) );
				m_CargoHeader.Update();
				
				if (m_AlternateFalseHeaderTextWidget)
				{
					m_AlternateFalseHeaderTextWidget.SetText(name);
				}
				return;
			}
		}
		
		if ( Container.Cast( GetParent() ) && Container.Cast( GetParent() ).GetHeader() )
			Container.Cast( GetParent() ).GetHeader().SetName(name);
	}
}
