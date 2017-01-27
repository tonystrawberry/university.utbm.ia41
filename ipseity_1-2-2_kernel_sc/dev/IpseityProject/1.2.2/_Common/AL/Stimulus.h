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


#ifndef Stimulus_h
#define Stimulus_h


#include "Types.h"
#include "AL/IpseityTypes.h"
#include "Math/Vector.h"


//! Stimulus perceived by an agent.
/*!
    A stimulus is represented by a tuple of sensory signals (transmitted along sensory channels).
    Each sensory signal is a vector whose components are of type \a IpseityPrecisionType (64-bit float).
    \author Fabrice LAURI
*/

class Stimulus
{
    public:
        //! Void stimulus. Its size is equal to 0.
        static Stimulus VoidStimulus;


    public:
        Stimulus();
        Stimulus( const Stimulus & s );
        Stimulus & operator=( const Stimulus & s );
        ~Stimulus();

        bool operator==( const Stimulus & ) const;

        uint32 getStatus() const;
        bool isTerminal() const;
        void setTerminal( bool );

        void setStatus( uint32 );
        bool isAGoal() const;
        void setGoal( bool );

        //! Get the number of sensory channels that compose the stimulus.
        /*!
         * \return Number of vectors.
         */
        uint32 getNbrSensoryChannels() const;

        //! Set the number of sensory channels that compose the stimulus.
        /*!
         * \param nbr_channels Number of sensory channels.
         */
        void setNbrSensoryChannels( uint32 nbr_channels );

        //! Select the sensory channel for subsequent updates or reads.
        /*!
         * \param channel_id Channel id.
         */
        void selectSensoryChannel( uint32 channel_id );

        //! Set the dimension of the current sensory channel.
        /*!
         * \param dim Dimension.
         */
        void setDimension( int32 dim );

        //! Get a sensory data from the current sensory channel.
        /*!
         * \param index Index of the component in the current sensory channel.
         * \return A scalar of type \a IpseityPrecisionType.
         */
        Ipseity::Scalar & operator[]( int32 index );

        //! Get the data from the current sensory channel.
        /*!
         * \return A vector of scalars of type \a IpseityPrecisionType.
         */
        const SensorySignal & getCurrentSensoryData() const;

        //! Set the data of the current sensory channel.
        /*!
         * \param vector New sensory data.
         */
        void setCurrentSensoryData( const SensorySignal & vector );

        //! Get the data from a given sensory channel.
        /*!
         * \param channel_id Id of the sensory channel.
         * \return A vector of scalars of type \a IpseityPrecisionType.
         */
        const SensorySignal & getSensoryData( uint32 channel_id ) const;

        //! Set the data of a given sensory channel.
        /*!
         * \param channel_id Id of the sensory channel.
         * \param vector New sensory data.
         */
        void setSensoryData( uint32 channel_id, const SensorySignal & vector );

        friend std::ostream & operator<<( std::ostream &, const Stimulus & );
        friend QDebug operator<<( QDebug, const Stimulus & );


    protected:
        union {
            struct {
                uint32	Terminal : 1,
                        Goal : 1;
            };

            uint32 m_Status;
        };
        uint32 m_ChannelId;
        uint32 m_Dimension;
        SensorySignal* m_Stimulus;
};


#endif
