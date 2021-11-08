class WindowBase
{
	public:
		enum MouseButton
		{
			MouseButtonLeft		= 1,
			MouseButtonMiddle	= 2,
			MouseButtonRight	= 4
		};

		// Constructor
		WindowBase();

		// Update
		virtual bool update() = 0;

		// Quit
		virtual void quit() = 0;

		// Get window width
		int getWidth() const {return m_iWidth;}

		// Get window height
		int getHeight() const {return m_iHeight;}

	protected:
		int	m_iWidth;		// Window size
		int	m_iHeight;
};
