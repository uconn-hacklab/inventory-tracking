#[cfg(feature = "ssr")]
pub mod ssr {

    use std::sync::{Arc, Mutex, RwLock};

    use sqlx::SqlitePool;

    #[derive(Debug)]
    pub struct AppState {
        pub pool: SqlitePool,
    }

    impl AppState {
        
    }

}
