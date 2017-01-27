/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "EnvironmentServer.h"


EnvironmentServer::EnvironmentServer(QObject *parent) : QTcpServer(parent)
{
	texts << tr("You've been leading a dog's life. Stay off the furniture.")
			 << tr("You've got to think about tomorrow.")
			 << tr("You will be surprised by a loud noise.")
			  << tr("You will feel hungry again in another hour.")
			  << tr("You might have mail.")
			  << tr("You cannot kill time without injuring eternity.")
			  << tr("Computers are not intelligent. They only think they are.");

	tcpSocket = NULL;
	n = 0;
}

void EnvironmentServer::incomingConnection(int socketDescriptor)
{
	tcpSocket = new QTcpSocket( this );
	connect( tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));

	if (tcpSocket->setSocketDescriptor(socketDescriptor))
	{
		std::cout << "New connection: " << socketDescriptor << std::endl;
//		addPendingConnection( tcpSocket );
	}
}

void EnvironmentServer::setEnvironment( AbstractEnvironment* env )
{
	m_Environment = env;

	if (m_Environment)
	{
		uint32 nbr_roles = m_Environment->getNbrRoles();

		std::cout << "# roles: " << nbr_roles << endl;

		for( uint32 i = 0; i < nbr_roles; ++i )
		{
			std::cout << i << ": " << m_Environment->getNbrStimulusVariables( i ) << "," << m_Environment->getNbrResponseVariables( i ) << endl;
		}
	}
}

void EnvironmentServer::readMessage()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_0);
	int n;

//	in >> blockSize;
//	std::cout << blockSize << std::endl;
	in >> n;

	cout << "Receive: " << n << endl;
}

void EnvironmentServer::sendMessage()
{
	if (tcpSocket && tcpSocket->isWritable() && tcpSocket->isOpen() && tcpSocket->isValid())
	{
		text = texts.at(n % texts.size());

		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
//		out << (quint16)0;
		out << text;
//		out.device()->seek(0);
//		out << (quint16)(block.size() - sizeof(quint16));

		std::cout << "Send: " << text.toStdString() << std::endl;

		tcpSocket->write( block );

		++n;
	}
}
