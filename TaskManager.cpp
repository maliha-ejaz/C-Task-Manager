#include<iostream>
#include<fstream>
#include<string>
#define MAX 100
using namespace std;

struct Task
{
	int id;
	string title;
	int priority;
	bool completed;
	Task* next;
};
Task* head = NULL;

struct Undo
{
	string action;
	int id;
	string title;
	int priority;
	bool completed;
};
Undo undoStack[MAX];
int top = -1;

void push(Undo u)
{
	if(top == MAX-1)
	{
		cout<<"Undo Stack Overflow!!"<<endl;
		return;
	}
	undoStack[++top] = u;
}
Undo pop()
{
	Undo u;
	if(top == -1)
	{
		cout<<"Nothing to undo."<<endl;
		u.action = "none";
		return u;
	}
	return undoStack[top--];
}

void addTask()
{
	Task* newTask = new Task;
	cout<<"Enter ID: ";
	cin>>newTask->id;
	cin.ignore();
	cout<<"Enter Title: ";
	getline(cin, newTask->title);
	cout<<"Enter Priority (1-High 2-Medium 3-Low): ";
	cin>>newTask->priority;
	
	newTask->completed = false;
	newTask->next = head;
	head = newTask;
	
	Undo u;
	u.action = "add";
	u.id = newTask->id;
	u.title = newTask->title;
	u.priority = newTask->priority;
	u.completed = newTask->completed;
	push(u);
	
	cout<<"Task Added Successfully."<<endl;
}
void deleteTask()
{
	int id;
	cout<<"Enter Task ID to Delete: ";
	cin>>id;
	Task* temp = head;
	Task* prev = NULL;
	
	while(temp && temp->id != id)
	{
		prev = temp;
		temp = temp->next;
	}
	
	if(!temp)
	{
		cout<<"Task Not Found!!"<<endl;
		return;
	}
	
	Undo u;
	u.action = "delete";
	u.id = temp->id;
	u.title = temp->title;
	u.priority = temp->priority;
	u.completed = temp->completed;
	push(u);
	
	if(!prev)
	{
		head = temp->next;
	}
	else
	{
		prev->next = temp->next;
	}
	delete temp;
	cout<<"Task Deleted Successfully."<<endl;
}
void markComplete()
{
	int id;
	cout<<"Enter Task ID to Mark Complete: ";
	cin>>id;
	
	Task* temp = head;
	while(temp && temp->id != id)
	{
		temp = temp->next;
	}
	if(!temp)
	{
		cout<<"Task Not Found."<<endl;
		return;
	}
	
	Undo u;
	u.action = "complete";
	u.id = temp->id;
	push(u);
	
	temp->completed = true;
	cout<<"Task Marked Completed."<<endl;
}
void viewPending()
{
	Task* temp = head;
	cout<<"--Pending Tasks--"<<endl;
	if(!temp)
	{
		cout<<"Nothing Pending"<<endl;
	}
	else
	{
		while(temp)
		{
			if(!temp->completed)
			{
				cout<<"ID: "<<temp->id<<endl;
				cout<<"Title: "<<temp->title<<endl;
				cout<<"Priority: "<<temp->priority<<endl;
				cout<<endl;
			}
			temp = temp->next;
		}
	}

}

void undoLast()
{
	Undo u = pop();
	if(u.action == "none")
	{
		return;
	}
	if(u.action == "add")
	{
		Task* temp = head;
		Task* prev = NULL;
		while(temp->id != u.id)
		{
			prev = temp;
			temp = temp->next;
		}
		if(temp)
		{
			if(!prev)
			{
				head = temp->next;
			}
			else
			{
				prev->next = temp->next;
				delete temp;
			}
		}
		cout<<"Undo Successful."<<endl;
	}
	else if(u.action == "delete")
	{
		Task* newTask = new Task;
		newTask->id = u.id;
		newTask->title = u.title;
		newTask->priority = u.priority;
		newTask->completed = u.completed;
		newTask->next = head;
		head = newTask;
		cout<<"Undo Successful."<<endl;
	}
	else if(u.action == "complete")
	{
		Task* temp = head;
		while(temp->id != u.id)
		{
			temp = temp->next;
		}
		if(temp)
		{
			temp->completed = false;
		}
		
		cout<<"Undo Successful."<<endl;
	}
	else
	{
		cout<<"No Action To Undo!"<<endl;
	}
}
void saveToFile()
{
	ofstream fout("tasks.txt");
    Task* temp = head;
    while(temp)
	{
		fout<<temp->id<<endl;
		fout<<temp->title<<endl;
		fout<<temp->priority<<endl;
		fout<<temp->completed<<endl;

        temp = temp->next;
    }
    fout.close();
    cout<<"Tasks saved.\n";
}
void viewAll()
{
    Task* temp = head;
    cout << "--All Tasks--" << endl;
    if (!temp)
	{
		cout<<"No tasks found."<<endl;
		return;
	}
    while (temp)
	{
        cout<<"ID: "<<temp->id<<endl;
        cout<<"Title: "<<temp->title<<endl;
        cout<<"Priority: "<<temp->priority<<endl;
        cout<<"Completed(0-No & 1-Yes): "<<temp->completed<<endl;
        cout<<endl;
        temp = temp->next;
    }
}
void loadFromFile()
{
	ifstream fin("tasks.txt");
    if (!fin)
    {
    	return;
	}

    while(head)
	{
        Task* temp = head;
        head = head->next;
        delete temp;
    }

    while (!fin.eof()) 
	{
        Task* newTask = new Task;
        char ch;
        if (!(fin >> newTask->id))
		{
			break;	
		}
		fin.ignore();

        getline(fin, newTask->title);
        fin>>newTask->priority;
        fin>>newTask->completed;
        fin.ignore();

        newTask->next = head;
        head = newTask;
    }

    fin.close();
}

int main()
{
	cout<<"[===TASK MANAGER===]"<<endl;
	cout<<"=++=To Do List=++="<<endl;
	loadFromFile();
	viewAll();
	
	while(1)
	{
		int choice;
		cout<<"=MENU="<<endl;
		cout<<"1. Add Task"<<endl;
		cout<<"2. Delete Task"<<endl;
		cout<<"3. Mark Complete"<<endl;
		cout<<"4. View Pending Tasks"<<endl;
		cout<<"5. Undo Last Action"<<endl;
		cout<<"6. Save Tasks"<<endl;
		cout<<"0. Exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		if(choice == 0)
		{
			cout<<"Exiting...."<<endl;
			break;
		}
		else if(choice == 1)
		{
			addTask();
		}
		else if(choice == 2)
		{
			deleteTask();
		}
		else if(choice == 3)
		{
			markComplete();
		}
		else if(choice == 4)
		{
			viewPending();
		}
		else if(choice == 5)
		{
			undoLast();
		}
		else if(choice == 6)
		{
			saveToFile();
		}
		else
		{
			cout<<"INVALID CHOICE!!!"<<endl;
		}
	}
	
	saveToFile();
	
	return 0;
}
