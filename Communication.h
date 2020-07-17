#pragma once

#include "stdafx.h"
#include "Messages.h"
#include "MpiConfig.h"
#include "Communication.h"

namespace COM
{
    bool logEnabled = true;
	bool logExtendedEnabled = true;
    Mutex logMutex;
    ofstream logFile("log" + std::to_string(MpiConfig::rank), std::ofstream::out | std::ofstream::trunc);

    void log(string message, MessageType type = MESSAGE_TYPES_COUNT)
    {
		if(logExtendedEnabled || message == "### All tasks completed! ###") 
		{
			logMutex.lock();
			int timestamp = Lamport::readTimestamp();

			if (logFile.is_open())
				logFile.close();

			logFile.open("log" + std::to_string(MpiConfig::rank), std::ofstream::app);
			
			
			
			if (logEnabled)
			{
				if(type == MESSAGE_TYPES_COUNT)
					cout << "T" << timestamp << "|P" << MpiConfig::rank << " : " << message << " " << endl;
				else
					cout << "T" << timestamp << "|P" << MpiConfig::rank << " : " << message << " " << Messages::getName(type) << endl;
			}
			
		
			if(type == MESSAGE_TYPES_COUNT)
				logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : " << message << " " << endl;
			else 
				logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : " << message << " " << Messages::getName(type) << endl;
			
			logFile.close();
			logMutex.unlock();
		}
    }

    void logSend(int target, const void *message, MessageType type)
    {
	    /*
        logMutex.lock();
        int timestamp = Lamport::readTimestamp();

        if (logFile.is_open())
            logFile.close();

        logFile.open("log" + std::to_string(MpiConfig::rank), std::ofstream::app);
		
		if(Messages::getName(type) == "REQUEST" || Messages::getName(type) == "RELEASE") {
			ResourceType resourceType = ((Request *)message)->type;
			string typeString = "";
			switch (resourceType)
			{
				case 100:
				{
					typeString = "zlecenie";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << endl;
							
					break;
				}
				case 200:
				{
					typeString = "agrafka";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << endl;
							
					break;
				}
				case 300:
				{
					typeString = "trucizna";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << " (units: " << ((Request *)message)->units << ")" << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "-> PROCESS " << target << ": " << Messages::getName(type) << " " << typeString << " (units: " << ((Request *)message)->units << ")" << endl;
							
					break;
				}
			}
			
		}
		else 
		{

			if (logEnabled)
			{
				cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
					 << "-> PROCESS " << target << ": " << Messages::getName(type) << endl;
			}
		
			logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
					<< "-> PROCESS " << target << ": " << Messages::getName(type) << endl;
		}
				
        logFile.close();
        logMutex.unlock();
	*/
    }

    void logReceive(int source, const void *message, MessageType type)
    {
	    /*
        logMutex.lock();
        int timestamp = Lamport::readTimestamp();

        if (logFile.is_open())
            logFile.close();

        logFile.open("log" + std::to_string(MpiConfig::rank), std::ofstream::app);
		
		if(Messages::getName(type) == "REQUEST" || Messages::getName(type) == "RELEASE") {
			ResourceType resourceType = ((Request *)message)->type;
			string typeString = "";
			switch (resourceType)
			{
				case 100:
				{
					typeString = "zlecenie";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " "
							 << "timestamp: " << ((Message *)message)->timestamp << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " "
							<< "timestamp: " << ((Message *)message)->timestamp << endl;
							
					break;
				}
				case 200:
				{
					typeString = "agrafka";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " "
							 << "timestamp: " << ((Message *)message)->timestamp << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " "
							<< "timestamp: " << ((Message *)message)->timestamp << endl;
							
							
					break;
				}
				case 300:
				{
					typeString = "trucizna";
					
					if (logEnabled)
					{
						cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							 << "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " (units: " << ((Request *)message)->units << ") "
							 << "timestamp: " << ((Message *)message)->timestamp << endl;
					}
					logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
							<< "<--- PROCESS " << source << ": " << Messages::getName(type) << " " << typeString << " (units: " << ((Request *)message)->units << ") "
							<< "timestamp: " << ((Message *)message)->timestamp << endl;
							
					break;
				}
			}
			
		}
		else 
		{
			if (logEnabled)
			{			
				cout << "T" << timestamp << "|P" << MpiConfig::rank << " : "
					 << "<--- PROCESS " << source << ": " << Messages::getName(type) << " "
					 << "timestamp: " << ((Message *)message)->timestamp << endl;
			}
			logFile << "T" << timestamp << "|P" << MpiConfig::rank << " : "
					<< "<--- PROCESS " << source << ": " << Messages::getName(type) << " "
					<< "timestamp: " << ((Message *)message)->timestamp << endl;
		}
		
        
        logFile.close();
        logMutex.unlock();
	*/
    }

    void send(int target, void *message, int type, int count = 1)
    {
        int result = MPI_Send(
            message,
            Messages::getSize(type) * count,
            MPI_BYTE,
            target,
            type,
            MPI_COMM_WORLD);
        if (result != MPI_SUCCESS)
        {
            cout << "Error sending to " << target << endl;
        }
        else
        {
            //logSend(target, message, (MessageType)type);
        }
    }

    void sendToAll(void *message, int type, int count = 1)
    {
        for (int i = 1; i < MpiConfig::size; i++) //ALERT!!!
        {
            if (i != MpiConfig::rank)
                send(i, message, type, count);
        }
    }

    void *receive(int type, int sender = MPI_ANY_SOURCE, int count = 1)
    {
        void *data = malloc(Messages::getSize(type) * count);

        int result = MPI_Recv(
            data,
            Messages::getSize(type) * count,
            MPI_BYTE,
            sender,
            type,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (result != MPI_SUCCESS)
        {
            cout << "Error receiving from " << sender << endl;
        }
        else
        {
            int timestamp, sender;

            timestamp = (*((Message *)data)).timestamp;
            sender = (*((Message *)data)).source;
            // if (Messages::castToMessage(type) == MESSAGE_COMPLETED)
            // {
            //     timestamp = 0;
            //     sender = 0;
            // }
            // else
            // {
            // timestamp = *(((int *)data + sizeof(int)));
            // sender = *(((int *)data));
            // // }
            //int timestamp = *(((int *)data + sizeof(int)));
            //int sender = *(((int *)data));
            // if (timestamp > 1000)
            // {
            //

            Lamport::update(timestamp); //wydobywam timestamp z wiadomosci
            //logReceive(sender, data, (MessageType)type);
        }

        return data;
    } // namespace COM
};    // namespace COM
