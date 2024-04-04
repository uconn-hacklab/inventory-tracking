from django import forms
from django.forms.widgets import HiddenInput
from django.conf import settings
from django.views.generic.dates import timezone_today

from .models import Item, LoanItem

class ItemForm(forms.ModelForm):
     class Meta:
         model = Item 
         fields = '__all__'

class BorrowForm(forms.ModelForm):
    due_date = forms.DateField(input_formats=settings.DATE_INPUT_FORMATS)

    class Meta:
        model = LoanItem
        fields = ['item', 'due_date','quantity'] 
    
    def __init__(self, *args, **kwargs):
        super(BorrowForm, self).__init__(*args, **kwargs)
        self.fields['item'].widget = HiddenInput()

    def clean_quantity(self):
        # Quantity cannot exceed amount of stock available
        quantity = self.cleaned_data['quantity']
        item = self.cleaned_data['item']
        print(item.n_loanable)
        if quantity > item.n_instock:
            raise forms.ValidationError("Quantity cannot exceed amount of stock available")

        if quantity <= 0:
            raise forms.ValidationError("Must loan at least one item")

        return quantity

    def clean_due_date(self):
        # Make sure date is later than today
        due_date = self.cleaned_data['due_date']

        if timezone_today() > due_date:
            raise forms.ValidationError("Cannot have a due date in the past")
        return due_date



