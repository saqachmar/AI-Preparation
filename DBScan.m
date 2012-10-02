%Soufiane Aq%
%DB Scan%
clc;
close all;

data=load('DataFile');


minimum_points_number=5; 
Density=1.9;  

neighbors_visited=zeros(size(data,1),1);
type=ones(size(data,1),1); 
clstr=zeros(size(data,1),1);
 count=0;
    for i=1:size(data,1)
        if neighbors_visited(i) 
        continue;
        end
        neighbors_visited(i)=1;
        
        [m,n]=size(data);
         Distance=sqrt(sum((ones(m,1)*data(i,:)-data).^2,2));
         indexes=find(Distance<=Density);
         index_of_neighbor=setdiff(indexes,i);      
        
        if length(index_of_neighbor)<minimum_points_number
            type(i)=-1;
        else
            count=count+1;
            type(i)=1;            
            clstr(i)=count;    
                j=1;
    while j<=length(index_of_neighbor)
        if ~neighbors_visited(index_of_neighbor(j))
            neighbors_visited(index_of_neighbor(j))=1;
            [m,n]=size(data);
            Distance=sqrt(sum((ones(m,1)*data(index_of_neighbor(j),:)-data).^2,2));
            indexes=find(Distance<=Density);
            neighbor2_index=setdiff(indexes,index_of_neighbor(j));  
            
            if length(neighbor2_index)>=minimum_points_number
                index_of_neighbor=([index_of_neighbor;neighbor2_index]);
                type(index_of_neighbor(j))=1;
            else                
                type(index_of_neighbor(j))=0;
            end
        elseif type(index_of_neighbor(j))==-1
            type(index_of_neighbor(j))=0;
        end
        if clstr(index_of_neighbor(j))==0
            clstr(index_of_neighbor(j))=count;
        end
        j=j+1;
    end

                       
            
        end
    end


 
Core=sum(type==1)
Border=sum(type==0)
Outlier=sum(type==-1)
    
clstring=(unique(clstr));

% Ploting the result can be done here ( to be done later )


 

