from django.contrib.auth.models import User
from django.db.models.query import F
from django.shortcuts import redirect, render, get_object_or_404
from django.views.generic import FormView, ListView, TemplateView
from django.contrib.auth.mixins import LoginRequiredMixin


from inventory.forms import ItemForm
from .models import *
from .forms import BorrowForm

# Create your views here.
from django.http import HttpResponseRedirect
from django.shortcuts import render

class OrgInventory(FormView):
    form_class = BorrowForm
    template_name = "org.html"
    success_url = "/thanks/"
    

    def get_queryset(self):
        self.org = get_object_or_404(Organization, slug=self.kwargs["slug"])
        return Item.objects.filter(organization=self.org)

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        self.org = get_object_or_404(Organization, slug=self.kwargs["slug"])
        context['object_list'] = Item.objects.filter(organization=self.org)
        context['org_name'] = self.org.name
        return context


    def form_valid(self, form):
        loan = form.save(commit=False)
        loan.recipient = self.request.user
        loan.save()
        
        return redirect('home')



class UserLoanInfo(TemplateView, LoginRequiredMixin):
    context_object_name = "user"
    model = User
    template_name = "user_loans.html"

    def all_loaned(self):
        # Get all items that have been borrowed ever
        return  

    def get_queryset(self):
        return self.request.user

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        all_loaned = LoanItem.objects.with_returns().filter(recipient=self.request.user).order_by('-due_date')
        context['waiting_approval'] = all_loaned.filter(approved=False)
        context['on_loan'] = all_loaned.filter(approved=True, n_returned__lt=F('quantity')) 
        context['history'] = all_loaned.filter(approved=True, n_returned=F('quantity'))
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
