// shortest job first process scheduling algorithm in operating systems
// the process file contains the list of processes start-time followed by pairs of cpu and io bursts.

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

using namespace std;

struct process
{
	int process_id;
	int start_time;
	int cpu_burst[10];
	int io_burst[10];
	int data_count;
}pr[10];

struct cpu
{
	int id;
	int duration;	
	cpu *next_cpu;
}*head_cpu=0,*null_cpu=0;

struct ready_queue
{
	process *pro;
	ready_queue *next_rq;
}*head_rq=0,*null_rq=0;

struct io_queue
{
	int id;
	int dispatch_time;
	io_queue *next_ioq;
}*head_ioq=0,*null_ioq=0;

int time_stamp=0,process_count;

void read_file()
{
	string line; //string declaration for getline function
	int i,j=0,k=0;
	process_count=0;
	ifstream file1("process");
	while(!file1.eof())
	{
		getline(file1,line); //getting each line with default parameter as null
		{
			int i=0;
			cout<<line<<"\n";
			while(line[i]!='\0')
			{
				if(line[i]==' ')
				{
					pr[j].data_count++;
					
				}
				i++;	//counter for character
			}
			j++;	//counter for line
		}

	}

	process_count=j-1;
	int cpu_no=0,io_no=0,temp;
	ifstream file2("process");
	
	for(i=0;i<process_count;i++)
	{	
		cpu_no=0,io_no=0;
		
		for(j=0;j<pr[i].data_count;j++)
		{
			file2>>temp;
			
				if(j==0)
				{
					pr[i].process_id=temp; //entering the process id
				}
				else if(j==1)
				{
					pr[i].start_time=temp; //entering the start time
				}
				
				else if((j%2)==0)
				{
					pr[i].cpu_burst[cpu_no]=temp; //entering the cpu burst array
					cpu_no++;
				}
				else
				{
					pr[i].io_burst[io_no]=temp; //entering the input output burst array
					io_no++;
				}
				
		}


	}

}

void push_to_readyqueue(process *pr,ready_queue **hd,int decider)     //push process to the ready queue
{
	
	ready_queue *new_rq,*temp,*prev;
	new_rq=(ready_queue *)malloc(sizeof(ready_queue));
	
	if(*hd==0) 					//empty ready queue
	{
		*hd=new_rq;   		    	//head is assigned
		new_rq->pro=pr; 	    	//direct insert
		new_rq->next_rq=null_rq;    //next pointer is made null
		return;
	}
	
	else if((**hd).next_rq==null_rq) 			 //only single node is present
	{
		if(decider==0)
		{
			new_rq->pro=pr;				       	 //direct insert
			new_rq->next_rq=null_rq; 		 	 //next pointer is made null
			(**hd).next_rq=new_rq;			 	 //next is assigned
			return;
		}
		else 
		{
			temp=(*hd);
			if( (pr->cpu_burst[0])<(temp->pro->cpu_burst[0]) )
			{
				*hd=new_rq;							 //head is assigned
				new_rq->pro=pr;				       	 //direct insert
				new_rq->next_rq=temp; 		 	     //next pointer is assigned
				
			}
			else
			{
				new_rq->pro=pr;				       	 //direct insert
				new_rq->next_rq=null_rq; 		 	 //next pointer is made null
				(**hd).next_rq=new_rq;			 	 //next is assigned
			}
			return;   		 			
		}
		
	}
	
	else 	 							 //non empty and non single queue 		
	{
		prev=(*hd);
		temp=(**hd).next_rq;
		
		if(decider==1)
		{
			if((pr->cpu_burst[0])<(prev->pro->cpu_burst[0]))
			{
				*hd=new_rq;							 //head is assigned
				new_rq->pro=pr;				       	 //direct insert
				new_rq->next_rq=prev; 		 	     //next pointer is assigned			
				return;	
			}
			
		}
		
		while( (pr->cpu_burst[0])>(temp->pro->cpu_burst[0]) )//the incoming process has greater cpu burst than already present
		{
			if(temp->next_rq==null_rq)				 //if next node is empty
			{
				new_rq->pro=pr;			     	 //direct insert
				new_rq->next_rq=null_rq; 			 //next pointer is made null
				temp->next_rq=new_rq;			 //next is assigned
				return;   		 
			}
			
			else 								 //shift for next condition check
			{
				prev=temp;
				temp=temp->next_rq;
			}
			
		}
		new_rq->pro=pr;			       	 //direct insert
		prev->next_rq=new_rq;			 //next of previous is assigned to new node
		new_rq->next_rq=temp;			 //next of new node is assigned to temp
		return;
		 
	}
	
}


void push_to_cpu(process *pr,cpu **hd)       					//push process to the cpu
{
	cpu *new_cpu,*temp;
	new_cpu=(cpu *)malloc(sizeof(cpu));
	if(*hd==0)									//empty cpu queue
	{
		*hd=new_cpu;
		new_cpu->id=pr->process_id;
		new_cpu->duration=1;
		new_cpu->next_cpu=null_cpu;

	}

	else
	{
		temp=*hd;
		while(temp->next_cpu!=null_cpu)
		{
			temp=temp->next_cpu;
		}
		new_cpu->id=pr->process_id;
		new_cpu->duration=1;
		new_cpu->next_cpu=null_cpu;
		temp->next_cpu=new_cpu;	

	}
}

void push_to_ioqueue(process *pr,io_queue **hd)
{
	io_queue *new_ioq,*temp;
	new_ioq=(io_queue *)malloc(sizeof(io_queue));
	if(*hd==0)									//empty cpu queue
	{
		*hd=new_ioq;
		new_ioq->id=pr->process_id;
		new_ioq->dispatch_time=time_stamp+(pr->io_burst[0]);
		new_ioq->next_ioq=null_ioq;
		return;

	}

	else
	{
		temp=*hd;
		while(temp->next_ioq!=null_ioq)
		{
			temp=temp->next_ioq;
		}
		new_ioq->id=pr->process_id;
		new_ioq->dispatch_time=time_stamp+(pr->io_burst[0]);
		new_ioq->next_ioq=null_ioq;
		temp->next_ioq=new_ioq;	
		return;
	}	
	
}

void check_ready(int decider)
{
	for(int i=0;i<process_count;i++)
	{
		if(pr[i].start_time==time_stamp)		//check if any cpu burst matches the time_stamp
		{
			push_to_readyqueue(&pr[i],&head_rq,decider);	    //push function invoked
		}
	}
	
	if(head_ioq!=0)
	{
    	if(head_ioq->dispatch_time<=time_stamp)		//check if any cpu burst matches the time_stamp
		{
			for(int i=0;i<process_count;i++)
			{
				if(pr[i].process_id==head_ioq->id)
				{
					push_to_readyqueue(&pr[i],&head_rq,decider);	    //push function invoked
					head_ioq=(head_ioq->next_ioq);
					return;
				}
			}
		
		}
	}
	
}


void assist(int decider)
{
	head_rq->pro->cpu_burst[0]--;	
	check_ready(decider);
	time_stamp++;
	return;
}

int check_for_exit()
{
	int flag=0;
	for(int i=0;i<process_count;i++)
	{
		if(pr[i].data_count>2)		
		{
			flag=1;	    
			return flag;
			
		}
	}
	return flag;
}

void run_process(int decider)
{

	cpu *temp;
		
	while(head_rq==0)                          //checking till ready queue becomes non empty
	{
		check_ready(decider);
		time_stamp++;
	}

	while(head_rq->pro->cpu_burst[0]>0)	      //checking till the cpu burst is exhausted
	{

		temp=head_cpu;

		if(temp==0)							  //empty cpu queue,first insertion
		{
			push_to_cpu(head_rq->pro,&head_cpu);
			assist(decider);
		}
		
		else                                        //non empty cpu queue
		{
			while(temp->next_cpu!=null_cpu)			//shifting till the last node in the cpu queue
			{
				temp=temp->next_cpu;				
			}
			
			if(head_rq->pro->process_id!=temp->id)		//push to cpu if the node encountered now is not the final node at cpu
			{	
				push_to_cpu(head_rq->pro,&head_cpu);
				assist(decider);
			}
			
			else 										//update the duration in the last cpu node
			{
				(temp->duration)++;

				assist(decider);
			}

		}

	}
	
	
	//once it has halted and when we want to push the next process to the cpu
	//if there are no process in the ready queue
    int flg;
	
	int count;

	//getting the data count
	count=head_rq->pro->data_count;
	count=(count-2)/2;
	
	//pop the latest cpu burst
	head_rq->pro->cpu_burst[0]=-1;		//invalidate the current cpu burst
	for(int i=0;i<(count-1);i++)
	{
		head_rq->pro->cpu_burst[i]=head_rq->pro->cpu_burst[i+1];	//shift the next cpu burst to the top
	}

	//move to the io queue

			time_stamp--;
			push_to_ioqueue(head_rq->pro,&head_ioq);					//pushing to the io queue
 			head_rq->pro->data_count-=2;									//decrease the data count
			time_stamp++;

	//pop the latest io burst
	for(int i=0;i<(count-1);i++)
	{
		head_rq->pro->io_burst[i]=head_rq->pro->io_burst[i+1];
	}	

	/*  move to the next node in the ready queue  */
	/*	even if head->next_rq is null there is no problem because
	 *  as null has value zero and that we are checking above for zero	*/

	head_rq=head_rq->next_rq;



	flg=check_for_exit();

	if(flg)
	{
		run_process(decider);
	}
	else
	{
		return;
	}

}

int main()
{
	int select;
	read_file();
	cout<<"\t\tSCHEDULING ALGORITHMS\n\n";
	cout<<"\t1.PREMPTIVE\n";
	cout<<"\t2.NON PREMPTIVE\n\n";
	cout<<"choice\n";
	cin>>select;
	cout<<"\nRESULT\n";
	if(select==2)
	select=0;
	run_process(select);
	cpu *temp;
	temp=head_cpu;
	while(temp->next_cpu!=null_cpu)
	{
	cout<<temp->id<<":"<<temp->duration<<"\n";	
	temp=temp->next_cpu;
	}
	cout<<temp->id<<":"<<temp->duration<<"\n\n";	
	return 0;

}
