use serde::{Deserialize, Serialize};

// TODO
// [ ] add item requests
// [ ] add purchase url link
#[derive(Serialize, Deserialize, Clone, Debug)]
#[cfg_attr(feature = "ssr", derive(sqlx::FromRow))]
pub struct Item {
    pub uuid: String,
    pub name: String,
    pub description: String,
    // pub tags: Vec<String>,
    // pub location: Location,
    // pub transactions: Vec<Transaction>,
}

impl Item {
    pub fn new(uuid: String, name: String, description: String) -> Item {
        Item {
            uuid,
            name,
            description,
        }
    }

}

#[cfg(feature="ssr")]
pub mod db {
    use crate::model::part::Item;
    use sqlx::{self, SqlitePool};

    impl Item {
        pub async fn insert(self, pool: &SqlitePool) -> Result<(), sqlx::Error> {
            sqlx::query("INSERT INTO item (uuid, name, description) VALUES (?, ?, ?)")
                .bind(self.uuid)
                .bind(self.name)
                .bind(self.description)
                .execute(pool)
                .await?;
            Ok(())
        }

        pub async fn get(id: String, pool: &SqlitePool) -> Option<Self> {
           sqlx::query_as::<_, Item>("SELECT * FROM item WHERE uuid=?").bind(id).fetch_one(pool).await.ok() 
        }
    }
}
#[derive(Serialize, Deserialize, Clone, Debug)]
pub enum TransactionMethod {
    ADD,
    LOST,
    BORROW,
}

#[derive(Serialize, Deserialize, Clone, Debug)]
pub struct Transaction {
    //pub user: User,
    pub method: TransactionMethod,
    pub component: Item,
    pub quantity: usize,
    pub comments: String,
}

// #[derive(Serialize, Deserialize, Clone, Debug)]
// pub struct Building(String);
//
// #[derive(Serialize, Deserialize, Clone, Debug)]
// pub struct Room(String);
//
// #[derive(Serialize, Deserialize, Clone, Debug)]
// pub struct StorageUnit(String);
//
// #[derive(Serialize, Deserialize, Clone, Debug)]
// pub struct Bin(String);
//
//
// #[derive(Serialize, Deserialize, Clone, Debug)]
// pub struct Location {
//     pub building: Building,
//     pub room: Room,
//     pub storage_unit: StorageUnit,
//     pub bin: Bin,
// }

