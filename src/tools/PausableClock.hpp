/*
  pausableclock
  Copyright (C) 2009 Marco Antognini (hiura@romandie.com)
  License : CC-BY-SA 3.0
  	You can find the full legal code at
  	http://creativecommons.org/licenses/by-sa/3.0/
  	or in the local file cc-by-sa-3.0-legalcode.html .
  	Here is only an abstract :

  You are free :
  	to Share — to copy, distribute and transmit the work
	to Remix — to adapt the work

  Under the following conditions :
  	Attribution. You must attribute the work in the manner
		specified by the author or licensor (but not
		in any way that suggests that they endorse you
		or your use of the work).
	Share Alike. If you alter, transform, or build upon this
		work, you may distribute the resulting work only
		under the same, similar or a compatible license.

  For any reuse or distribution, you must make clear to others
  	the license terms of this work. The best way to do this
	is with a link to this
       	(http://creativecommons.org/licenses/by-sa/3.0/) web page.

  Any of the above conditions can be waived if you get
  	permission from the copyright holder.

  Nothing in this license impairs or restricts the author's
  	moral rights.

  SPECIAL THANKS TO SFML LIBRARY (See http://sfml-dev.org)
  AND to Laurent Gomila.

*/

/*
Modified by Pierreyoda (documentation, functions names, well-working with luabind...), 2010.
Original work from Hiura.
*/

#ifndef PAUSABLECLOCK_HPP
#define PAUSABLECLOCK_HPP

#include <SFML/System.hpp>

/** \brief sf::Clock with pause function.
*
* \see http://www.sfml-dev.org/wiki/en/sources/pausableclock (English)
*/
class PausableClock
{
	public :
        /** \brief Default constructor.
        */
        PausableClock() : m_paused(false), m_time(0.f), m_clock()
        { }
		/** \brief Other constructor.
		*
        * \param paused Does the clock start paused?
		*/
		PausableClock(const bool &paused) : m_paused(paused), m_time(0.f),
            m_clock()
		{ }

		/** \brief Set the clock in 'pause' mode.
		*/
		void pause()
		{
			if (!m_paused) // If not yet paused...
			{
				m_paused = true;
				m_time += m_clock.GetElapsedTime();
			}
		}

		/** \brief Start the clock (again).
		*/
		void start()
		{
			if (m_paused) // If not yet started...
			{
				m_clock.Reset();
				m_paused = false;
			}
		}

		/** \brief Get the time elapsed since last Reset.
		*/
		float getElapsedTime() const
		{
			if (!m_paused) // If not paused...
				return m_time + m_clock.GetElapsedTime();
			else
				return m_time;
		}

        /** \brief Reset the clock.
		*/
		void reset()
		{
			reset(false);
		}
		/** \brief Reset the clock.
		*
        * \param paused : does the clock start paused ?
		*/
		void reset(const bool &paused)
		{
			m_paused = paused;
			m_time = 0.f;
			m_clock.Reset();
		}

        /** \brief Is anim paused?
        *
        * \return m_paused.
        * \see m_paused
        */
		bool isPaused() const { return m_paused; }

	private :
		bool m_paused; /**< Is the PausableClock paused ? */
		float m_time; /**< Time buffer. */
		sf::Clock m_clock; /**< The sf::Clock used. */
};

#endif /* PAUSABLECLOCK_HPP */
