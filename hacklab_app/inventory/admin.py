from django.contrib import admin
from .models import *


# Register your models here.
admin.site.register(Tag)
admin.site.register(Building)
admin.site.register(Room)
admin.site.register(StorageUnit)
admin.site.register(Organization)

admin.site.register(ReturnItem)
admin.site.register(AddItem)
admin.site.register(LoseItem)


@admin.register(Item)
class ItemAdmin(admin.ModelAdmin):
    readonly_fields = ('uuid', 'n_items', 'n_lost', 'n_instock')
    list_display = ('__str__', 'n_items', 'n_instock', 'n_borrowed', 'n_lost')
    

@admin.register(LoanItem)
class TransactionAdmin(admin.ModelAdmin):
    list_display = ('__str__', 'quantity', 'date', ) 
    readonly_fields = ()
    
    def save_model(self, request, obj, form, change):
        if not request.user.is_staff:
            raise PermissionDenied("User must be part of the staff in order to approve a transaction")

        obj.approvee = request.user
        super().save_model(request, obj, form, change)

