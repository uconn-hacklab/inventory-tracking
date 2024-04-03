from django.contrib.auth.models import User
from django.shortcuts import render, get_object_or_404
from django.views.generic import DetailView, ListView, TemplateView, View
from django.contrib.auth.mixins import LoginRequiredMixin

from inventory.forms import ItemForm
from .models import *

# Create your views here.
from django.http import HttpResponseRedirect
from django.shortcuts import render

class OrgInventory(ListView):
    context_object_name = "item_list"
    template_name = "org.html"

    def get_queryset(self):
        self.org = get_object_or_404(Organization, name=self.kwargs["name"])
        return Item.objects.filter(organization=self.org)

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)

        context['org_name'] = self.org.name
        return context

class UserLoanInfo(TemplateView, LoginRequiredMixin):
    context_object_name = "user"
    model = User
    template_name = "user_loans.html"

    def all_loaned(self):
        # Get all items that have been borrowed ever
        return LoanItem.objects.filter(recipient=self.request.user) 

    def get_queryset(self):
        return self.request.user

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['all_loaned'] = self.all_loaned()
        return context

def get_name(request):
    # if this is a POST request we need to process the form data
    if request.method == "POST":
        # create a form instance and populate it with data from the request:
        form = ItemForm(request.POST)
        # check whether it's valid:
        if form.is_valid():
            # process the data in form.cleaned_data as required
            # ...
            # redirect to a new URL:
            return HttpResponseRedirect("/thanks/")

    # if a GET (or any other method) we'll create a blank form
    else:
        form = ItemForm()

    return render(request, "item.html", {"form": form})
