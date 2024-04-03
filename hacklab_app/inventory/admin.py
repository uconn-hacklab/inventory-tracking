from django.contrib import admin
from .models import *


# Register your models here.
admin.site.register(Tag)
admin.site.register(Building)
admin.site.register(Room)
admin.site.register(StorageUnit)
admin.site.register(Organization)
admin.site.register(LoanItem)

admin.site.register(ReturnItem)
admin.site.register(AddItem)
admin.site.register(LoseItem)


@admin.register(Item)
class ItemAdmin(admin.ModelAdmin):
    readonly_fields = ('uuid', 'n_items', 'n_lost', 'n_instock')
    list_display = ('__str__', 'n_items', 'n_instock', 'n_borrowed', 'n_lost')
    

@admin.register(Transaction)
class TransactionAdmin(admin.ModelAdmin):
    list_display = ('__str__', 'quantity', 'date', ) 
    readonly_fields = ()

