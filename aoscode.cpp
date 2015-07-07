#include<iostream.h>
#include<conio.h>
#include<stdlib.h>
#include<fstream.h>


struct log
{
	int aid;
	int did;
	int value;
};
struct lock
{
	int did;
	int status;
	lock *lnext;
};
struct action
{
    int id;
    int state;
    action *next;
};
struct data
{
    int dataid;
    int value;
    int actionid;
    data *vnext;
};
int checklock(lock *head, int did)
{
	int check=0;
	while(head!=NULL)
	{
		if(head->did == did)
		{
			if(head->status == 2)
			{
				check = 1;
				break;
			}
		}
		head = head->lnext ;
	}
	return check;
}
void afterop(lock *head, int did)
{
	while(head!=NULL)
	{
		if(head->did == did)
		{
			head->status = 1;
			break;
		}
		head = head->lnext;
	}
}
void lockupdate(lock *head, int did, int op)
{
   lock *tl;
   tl=head;
   int temp=0;
   cout<<"Inlockupdate  ";
   while(tl!=NULL)
   {
	cout<<"While loop  "<<" value "<<tl->did<<" "<<did<<endl;
	if(tl->did == did)
		{
			cout<<"If condition "<<endl;
			temp=1;
			break;
		}
	tl = tl->lnext;
   }
   cout<<endl<<" value of temp  :"<<temp<<endl;
   if(temp == 0)
   {
	lock *ql;
	ql = new lock;
	cout<<"  Value of did passed by function "<< did;
	ql->did = did;
	cout<<endl<<"value of did after assignment "<<ql->did;
	if(op == 1)
	{
		ql->status = 1;
	}
	else
	{
		ql->status = 2;
	}
	ql->lnext = head->lnext;
	head->lnext = ql;

   }
   else if (temp == 1)
   {
	if(op == 1)
	{
		tl->status = 1;
	}
	else if(op == 2)
	{
		tl->status = 2;
	}
   }
   cout<<" End ";
}
void new_data(data *head, int val,int aid,int did)
{
    data *q;
    q=new data;
    q->dataid = did;
    q->value = val;
    q->actionid = aid;
    q->vnext = head->vnext;
    head->vnext = q;
}

int new_action(action *h, int a)
{

    action *c;
    c = new action;
    c->id = a;
    c->state = 1;
    c->next = h->next;
    h->next = c;
    return a;

}
void traverse(action *h)
{
    while(h!=NULL)
    {
	cout<<endl<<h->id<<" "<<h->state;
	h = h->next;
    }
}
void traverseA(data *hq)
{
    while(hq!=NULL)
    {
	cout<<endl<<" "<<hq->dataid<<" "<<hq->value<<" "<<hq->actionid;
	hq = hq->vnext;
    }
}
void commitee(action *hw, data *ha, int aid)
{
     log lg;
     while(ha!=NULL)
     {
	if(ha->actionid == aid)
	{
		lg.did = ha->dataid;
		lg.aid = ha->actionid;
		lg.value = ha->value;
		break;
	}
	ha = ha->vnext;
     }
     ofstream f("logentry", ios::out | ios::binary);
     if(!f){
	cout<<"something wrong with file "<<endl;
     }

     f.write((char *) &lg,sizeof(log));
     f.close();

     while(hw!=NULL)
	{

	    if(hw->id == aid && hw->state == 1)
		{
		    hw->state = 2;

		}
	    hw= hw->next;
	}
}
void abortee(action *hw,int aid)
{
     while(hw!=NULL)
	{
	    if(hw->id == aid && hw->state == 1)
		{
		    hw->state = 3;
		}
	    hw= hw->next;
	}
}
void read_current_value(data *he,action *hr,int did)
{

    int actid = 0;

    while(hr!=NULL)
	{

		if(hr->state == 2)
		{
		    actid=hr->id;
		    break;
		}
	    hr=hr->next;
	}
    while(he!=NULL)
    {

	if(he->actionid==actid && he->dataid==did)
	{
	    cout<<endl<<"Last committed value of data Id    :"<<he->value;
	    break;
	}
	he=he->vnext;
    }

}
void recovery(data *hd, lock *hl, action *ha)
{
   ifstream inf("logentry", ios::in | ios::binary);
   if(!inf)
   {
	cout<<"something wrong with a file "<<endl;
   }
   else
   {
	log recover, lg1;
	inf.read((char *) &recover, sizeof(log));
	ofstream fout("logentry1",ios::out | ios::binary);
	int a;
	while(!inf.eof())
	{
		inf.read((char *) &recover, sizeof(log));
		a = new_action(ha,recover.aid);
		lockupdate(hl, recover.did , 1);
		new_data(hd, recover.value, recover.aid , recover.did);
		commitee(ha,hd,recover.aid);
		lg1.aid=recover.aid;
		lg1.did=recover.did;
		lg1.value=recover.value;
		fout.write((char *)&lg1, sizeof(log));
	}
	fout.close();
	inf.close();
	remove("logentry");
	rename("logentry1","logentry");
   }
}
int main()
{
    system("cls");
    int aut = 0 ;
    lock *hl, *nl;
    nl = new lock;
    nl->did = 0;
    nl->status = 0;
    nl->lnext = NULL;
    hl=nl;
    action *h, *n;
    n = new action;
    n->id = 0;
    n->state = 0;
    n->next = NULL;
    h=n;
    data *ha, *na;
    na = new data;
    na->dataid = 0;
    na->value = 0;
    na->actionid =  0;
    na->vnext = NULL;
    ha=na;
    int check=0;
    int aid=-1;
    int did=-1;

L:
    cout<<endl<<"1. New Action ";
    cout<<endl<<"2. Write a value ";
    cout<<endl<<"3. commit ";
    cout<<endl<<"4. Abort ";
    cout<<endl<<"5. Read a value ";
    cout<<endl<<"6. Recovery ";
    cout<<endl<<"7. Exit ";
    cout<<endl<<"Please Enter your choice(reversed keywords 0,1,2,3)   : ";
    int x;
    cin>>x;

    switch(x)
    {
    case 1:
	aid = new_action(h,++aut);

	goto L;
    case 2:
	int val;
	cout<<endl<<"Please enter data id         :";
	cin>>did;
	cout<<"please enter a value to write   :";
	cin>>val;
	check = checklock(hl,did);
	if(check == 0)
	{
		lockupdate(hl, did, 2);
		new_data(ha, val, aid , did);
		afterop(hl, did);
	}
	else
	{
		cout<<"Process going on";
	}
	goto L;
    case 3:
	commitee(h,ha,aid);
	goto L;
    case 4:
	abortee(h,aid);
	goto L;
    case 5:
	cout<<endl<<"reading current value   ";
	cout<<endl<<"please enter data      :";
	cin>>did;
	check = checklock(hl, did);
	if(check == 0)
	{
		lockupdate(hl, did, 1);
		read_current_value(ha,h,did);
		afterop(hl,did);
	}
	else
	{
		cout<<"Writing is going on";
	}
	goto L;
    case 6:
       recovery(ha,hl,h);
       goto L;
    case 7:
       return 0;
    }
    /*int aid;
    aid = new_action(h,++aut);
    aid = new_action(h,++aut);
    aid = new_action(h,++aut);
    commitee(h,2);
    abortee(h,3);
    traverse(h);


    new_data(ha, 10, aid);
    new_data(ha, 11, 2);
    new_data(ha, 12, aid);
    traverseA(ha);

    read_current_value(ha,h);

    cin.get();
    */
    return 0;

}