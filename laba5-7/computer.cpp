#include "computer.hpp"

int main (int argc, char const *argv[]){
	std::map<std::string, int> local_map;//хранилище
	char * path_to_computer = getenv("PATH_TO_COMPUTER");//путь к файлу из окружения
	int my_id = std::stoi(std::string(argv[1])); //ид текущего процесса
	ConnectingSocket parent_socket(my_id);
	std::cout << "OK: " << getpid() << std::endl;
	Tree t(my_id);
	std::map<int, Socket *> children;//хранилище дочерних процессов
	Msg parent_msg;
	Msg new_msg;
	Msg reply;
	BindingSocket * new_socket;
	int alive = 1;
	while(alive){
		parent_socket.recvMsg(parent_msg);
		if (parent_msg.action == connect){
			parent_msg.action = success;
		} else if (parent_msg.action == create){
			parent_msg.action = success;
			int parent_id = parent_msg.arg1, child_id = parent_msg.arg2;
			if (parent_id == my_id){
				new_socket = new BindingSocket(child_id);
				children.emplace(child_id, new_socket);
				int child_pid = fork();
				if (child_pid == 0){
					execl(path_to_computer, path_to_computer, std::to_string(child_id).c_str(), nullptr);
					return 0;
				} else{
					new_msg.action = connect;
					if (children[child_id]->sendRecvMsg(&new_msg, reply) and reply.action == success){
						t.push(my_id, child_id);
					} else{
						children.erase(child_id);
					}
				}
			} else{
				int root_child = t.findRootChild(parent_id);//нахождение корневого дочернего процесса
				new_msg.action = create;
				new_msg.setMsgArgs(parent_id, child_id);
				if (children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) and reply.action == success){
					t.push(parent_id, child_id);
				} else{
					parent_msg.action = fail;
				}
			}
		} else if(parent_msg.action == check){
			parent_msg.action = success;
			int id = parent_msg.arg2;
			std::string key;
			key += parent_msg.arg1;
			int rd = 1;
			parent_socket.sendMsg(&parent_msg);
			while(rd){
				parent_socket.recvMsg(parent_msg);
				if(parent_msg.arg1 == '#'){
					rd = 0;
				} else{
					key += parent_msg.arg1;
				}
				parent_msg.action = success;
				parent_socket.sendMsg(&parent_msg);
			}
			if (id == my_id){
				std::cout << "Ok: " << my_id << ": ";
				if (local_map.find(key) != local_map.end()){
					std::cout << local_map[key] << std::endl;
				} else{
					std::cout << key << " not found" << std::endl;
				}
			} else{
				new_msg.action = check;
				key += '#';
				int root_child = t.findRootChild(id);
				new_msg.arg2 = id;
				for (auto i: key){
					new_msg.arg1 = i;
					if (!children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) or reply.action == fail){
						std::cout << "Error:id: Node is unavailable" << std::endl;
						break;
					}
				}
			}
			continue;
		} else if (parent_msg.action == add){
			parent_msg.action = success;
			int id = parent_msg.arg2;
			int value;
			std::string key;
			key += parent_msg.arg1;
			int rd = 1;
			parent_socket.sendMsg(&parent_msg);
			while(rd){
				parent_socket.recvMsg(parent_msg);
				if(parent_msg.arg1 == '#'){
					rd = 0;
					value = parent_msg.arg2;
				} else{
					key += parent_msg.arg1;
				}
				parent_socket.sendMsg(&parent_msg);
			}
			if (id == my_id){
				std::cout << "Ok: " << my_id << std::endl;
				local_map.emplace(key, value);
			} else{
				new_msg.action = add;
				key += '#';
				int root_child = t.findRootChild(id);
				new_msg.arg2 = id;
				for (auto i: key){
					new_msg.arg1 = i;
					if (i == '#'){
						new_msg.arg2 = value;
					}
					if (!children[root_child]->sendNoWaitRecvMsg(&new_msg, reply) or reply.action == fail){
						std::cout << "Error:id: Node is unavailable" << std::endl;
					}
				}
			}
			continue; 
		} else if(parent_msg.action == die){
			alive = 0;
		}else if (parent_msg.action = ping){
			std::vector<int> busy_processes;
			for(auto p : children){
				new_msg.action = ping;
				if(p.second->sendNoWaitRecvMsg(&new_msg, reply)){
					while (reply.action == busy){
						p.second->sendMsg(&new_msg);
						busy_processes.push_back(reply.arg1);
						p.second->recvMsg(reply);
					}
				} else{
					t.find(p.first)->traversal(busy_processes);
				}
			}
			for (auto i : busy_processes){
				parent_msg.action = busy;
				parent_msg.arg1 = i;
				parent_socket.sendMsg(&parent_msg);
				parent_socket.recvMsg(reply);					
			}
			parent_msg.action = success;
		}
		parent_socket.sendMsgNoWait(&parent_msg);
	}
	new_msg.action = die;
	for(auto p : children){
		p.second->sendRecvMsg(&new_msg, reply);
	}
	return 0;
}
