use std::sync::{Arc, Mutex};

#[derive(Default)]
pub struct TransferManager {
    pub jobs: Arc<Mutex<Vec<TransferJob>>>,
}

use crate::transfers::job::TransferJob;

impl TransferManager {
    pub fn new() -> Self { Self::default() }
    pub fn add(&self, job: TransferJob) { self.jobs.lock().unwrap().push(job); }
}
