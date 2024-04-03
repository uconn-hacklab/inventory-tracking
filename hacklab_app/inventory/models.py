from django.db import models
from django.contrib.auth.models import User
from django.db.models import Sum

from django.db.models.functions import Coalesce
import uuid


from django.core.exceptions import PermissionDenied, ValidationError

class Building(models.Model):
    name = models.CharField(max_length=30)
    description = models.TextField()
    

class Room(models.Model):
    name = models.CharField(max_length=30)
    description = models.TextField()
    building = models.ForeignKey(Building, on_delete=models.CASCADE)
    

class StorageUnit(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField()
    room = models.ForeignKey(Room, on_delete=models.CASCADE)
    


class Tag(models.Model):
    name = models.CharField(max_length=20)

# An organization that has items that belong to them
class Organization(models.Model):
    name = models.CharField(max_length=25, unique=True)
    description = models.TextField(null=True, blank=True)

    def __str__(self):
        return str(self.name)


class Item(models.Model):
    uuid = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)
    organization = models.ForeignKey(Organization, on_delete=models.CASCADE)

    name = models.CharField(max_length=100)
    description = models.TextField(null=True, blank=True)
    tags = models.ManyToManyField(Tag, blank=True)
    
    @property
    def n_items(self):
        """Returns the total number of items owned by the organization (including lost)"""
        return AddItem.objects.filter(item=self).aggregate(count=Coalesce(Sum('quantity'), 0))['count']

    @property
    def n_lost(self):
        """Returns the number of items that are lost"""
        return LoseItem.objects.filter(item=self).aggregate(count=Coalesce(Sum('quantity'), 0))['count'] 

    @property
    def n_borrowed(self):
        """Returns the number of items currently being borrowed"""
        loans = LoanItem.objects.filter(item=self)
        n_loaned = loans.aggregate(count=Coalesce(Sum('quantity'), 0))['count']

        n_returned = loans.aggregate(count=Coalesce(Sum('returns__quantity'), 0))['count']
        return n_loaned - n_returned 

    @property
    def n_instock(self):
        """Returns the total number of this item available for borrowing"""
        return self.n_items - self.n_borrowed - self.n_lost 

    @property
    def n_loanable(self):
        """Returns that number of items that could be loaned if everything is returned"""
        return self.n_items - self.n_lost

    
    def __str__(self):
        return f"{self.name} ({self.uuid})" 


class Transaction(models.Model):
    item = models.ForeignKey('Item', on_delete=models.CASCADE) # which items the user is checking out
    quantity = models.IntegerField()
    date = models.DateTimeField(auto_now_add=True)

    approvee = models.ForeignKey(User, related_name="approvee", on_delete=models.PROTECT)
    description = models.TextField()

    def save(self, *args, **kwargs) -> None:
        user = kwargs.pop('user', None)

        if user and not self.approvee.is_staff:
            raise PermissionDenied("User must be part of the staff in order to approve a transaction")

        return super().save(*args, **kwargs)


class LoseItem(Transaction):
    def __str__(self):
        return f"LOST: {self.description[0:30]}"

class AddItem(Transaction):
    def __str__(self):
        return f"ADD: {self.description[0:30]}"

class ReturnItem(Transaction):
    def __str__(self):
        return f"RETURN: {self.description[0:30]}"

class LoanItem(Transaction):
    # A loan can have transactions that return items
    # TODO ensure that these transactions are only returns
    recipient = models.ForeignKey(User, on_delete=models.PROTECT)
    returns = models.ManyToManyField(ReturnItem, blank=True, related_name="return_trans")

    @property
    def n_returned(self) -> int:
        count = self.returns.aggregate(n_returned=Sum("quantity"))
        if count.n_returned is None:
            return 0
        return count.n_returned

    @property
    def is_returned(self) -> bool:
        return self.n_returned  == self.quantity

    def __str__(self):
        return f"LOAN: {self.description[0:30]}"

        
