from django import forms
from django.forms.widgets import HiddenInput

from .models import Item, LoanItem

class ItemForm(forms.ModelForm):
     class Meta:
         model = Item 
         fields = '__all__'

class BorrowForm(forms.ModelForm):
    class Meta:
        model = LoanItem
        fields = ['item', 'due_date','quantity'] 
    
    def __init__(self, *args, **kwargs):
        super(BorrowForm, self).__init__(*args, **kwargs)
        self.fields['item'].widget = HiddenInput()



